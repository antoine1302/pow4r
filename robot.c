#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "robot.h"
#include "grid.h"
#include "position.h"
#include "rule.h"
#include "player.h"
#include "random.h"

static unsigned findMaxAlignmentByPosition(struct position *, unsigned, unsigned (*)[GRID_WIDTH]);
static unsigned findMaxAlignmentByDirection(struct position, int , int, unsigned, unsigned (*)[GRID_WIDTH]);
static _Bool isPositionValid(struct position *);
static unsigned get_max(unsigned, ...);

void get_robot_input(unsigned *column, unsigned (*grid)[GRID_WIDTH])
{
    struct position current_pos = {.x = 0, .y = 0};
    unsigned max_alignment = 0, global_max_alignment = 0, count_per_alignment = 0;
    unsigned best_column[GRID_WIDTH] = {0};

    for (unsigned i = 0; i < GRID_WIDTH; i++) {
        // column is already full
        if (grid[0][i] != 0) {
            continue;
        }

        current_pos.x = i;

        for (int height = GRID_HEIGTH -1; height >= 0; height--) {
            if (grid[height][i] == 0) {
                current_pos.y = height;
                break;
            }
        }

        max_alignment = findMaxAlignmentByPosition(&current_pos, PLAYER_2_ID, grid);

        if (max_alignment >= WIN_ALIGNMENT) {
            *column = i;
            return;
        }

        max_alignment = get_max(max_alignment, findMaxAlignmentByPosition(&current_pos, PLAYER_1_ID, grid));

        if (max_alignment >= global_max_alignment)
        {
            if (max_alignment > global_max_alignment)
            {
                count_per_alignment = 0;
                global_max_alignment = max_alignment;
            }

            best_column[count_per_alignment++] = i;
        }
    }

    *column = best_column[random_int(0, count_per_alignment - 1)];
}

unsigned findMaxAlignmentByPosition(struct position *pos, unsigned player_id, unsigned (*grid)[GRID_WIDTH])
{
    return  get_max(
        findMaxAlignmentByDirection(*pos, 1, 0, player_id, grid),
        findMaxAlignmentByDirection(*pos, 0, 1, player_id, grid) + findMaxAlignmentByDirection(*pos, 0, -1, player_id, grid) - 1,
        findMaxAlignmentByDirection(*pos, 1, 1, player_id, grid) + findMaxAlignmentByDirection(*pos, -1, -1, player_id, grid) - 1,
        findMaxAlignmentByDirection(*pos, 1, -1, player_id, grid) + findMaxAlignmentByDirection(*pos, -1, 1, player_id, grid) - 1
        );
}

unsigned findMaxAlignmentByDirection(struct position pos, int move_vertical, int move_horizontal, unsigned player_id,  unsigned (*grid)[GRID_WIDTH])
{
    pos.x += move_horizontal;
    pos.y += move_vertical;
    unsigned max_alignment = 1;

    while (isPositionValid(&pos)) {
        if (grid[pos.y][pos.x] == player_id) {
            max_alignment++;
        } else {
            break;
        }

        pos.x += move_horizontal;
        pos.y += move_vertical;
    }

    return max_alignment;
}

_Bool isPositionValid(struct position *pos)
{
    if (pos->x >= GRID_WIDTH || pos->x < 0) {
        return 0;
    } else if (pos->y >= GRID_HEIGTH || pos->y < 0) {
        return 0;
    }

    return 1;
}

unsigned get_max(unsigned arg_count, ...)
{
    va_list ap;
    va_start(ap, arg_count);
    unsigned current_val = 0;
    unsigned max = va_arg(ap, unsigned);

    for (unsigned i = 2; i <= arg_count; i++) {
        if ((current_val = va_arg(ap, unsigned)) > max) {
            max = current_val;
        }
    }

    va_end(ap);

    return max;
}
