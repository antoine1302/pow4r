#include <stdio.h>
#include <stdlib.h>
#include "robot.h"
#include "grid.h"
#include "position.h"
#include "rule.h"
#include "random.h"

static unsigned get_max_align(struct position *, unsigned (*)[GRID_WIDTH]);
static int get_best_column(int *);

void get_robot_input(unsigned *column, unsigned (*grid)[GRID_WIDTH])
{
    int max_align_per_column[GRID_WIDTH] = {0};

    for (unsigned i = 0; i < GRID_WIDTH; i++) {
        for (unsigned j = 0; j < GRID_HEIGTH; j++) {
            if(grid[i][j] == 0 && j < GRID_HEIGTH - 1) {
                continue;
            } else if (j == 0) {
                break;
            } else {
                max_align_per_column[i] = get_max_align(&(struct position) { .x = i, .y = j }, grid);
                break;
            }
        }
    }

    *column = get_best_column(max_align_per_column);
}

unsigned get_max_align(struct position *pos, unsigned (*grid)[GRID_WIDTH])
{
    unsigned max_align = 1;
    unsigned max_align_vertical = 1;
    unsigned max_align_horizontal = 1;
    // unsigned max_align_oblique_to_right = 1;
    // unsigned max_align_oblique_to_left = 1;

    unsigned player_id = grid[pos->x][pos->y];

    // column is empty
    if (player_id != 0) {
        // check max align vertical
        for (unsigned i = pos->y + 1; i < GRID_HEIGTH; i++) {
            if (player_id == grid[pos->x][i]) {
                max_align_vertical++;
            } else {
                break;
            }
        }
    }

    if (max_align_vertical > max_align) {
        max_align = max_align_vertical;
    }

    //check max align horizontal
    for (unsigned i = pos->x + 1; i < GRID_WIDTH; i++) {
        if (player_id == grid[i][pos->y]) {
            max_align_horizontal++;
        } else {
            break;
        }
    }

    for (int i = pos->x - 1; i > 0; i--) {
        if (player_id == grid[i][pos->y]) {
            max_align_horizontal++;
        } else {
            break;
        }
    }

    if (max_align_horizontal > max_align) {
        max_align = max_align_horizontal;
    }

    //check max align oblique right
    //check max align oblique left


    return max_align;
}

int get_best_column(int *max_align_per_column)
{
    for (int align_count = WIN_ALIGNMENT; align_count > 0; align_count--) {
        int align_count = 0;
        int column_per_align[GRID_WIDTH] = {0};
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (max_align_per_column[j] >= align_count) {
                column_per_align[align_count++] = j;
            }
        }

        if (align_count > 0) {
            if (align_count == 1) {
                return column_per_align[1];
            }

            return random_int(0, align_count - 1);
        }
    }

    return 0;
}
