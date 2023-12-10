#include <stdio.h>
#include <stdlib.h>
#include "robot.h"
#include "grid.h"
#include "position.h"
#include "rule.h"
#include "random.h"

static int get_best_column(int *);
static unsigned get_max_align(unsigned (*grid)[GRID_WIDTH], struct position *);

void get_robot_input(unsigned *column, unsigned (*grid)[GRID_WIDTH])
{
    int max_align_per_column[GRID_WIDTH] = {0};

    for (unsigned i = 0; i < GRID_WIDTH; i++) {
        for (unsigned j = 0; j < GRID_HEIGTH; j++) {
            if (grid[j][i] == 0) {
                // reached the bottom of the column
                if (j == 5) {
                    max_align_per_column[i] =  get_max_align(grid, &(struct position){.x = i, .y = j});
                    break;   
                } else {
                    continue;
                }
            } else {
                // column is full 
                if (j == 0) {
                    break;
                } else {
                    max_align_per_column[i] =  get_max_align(grid, &(struct position){.x = i, .y = j -1});
                    break;   
                }
            }
        }
    }

    *column = get_best_column(max_align_per_column) + 1;
}

unsigned get_max_align(unsigned (*grid)[GRID_WIDTH], struct position *position)
{
    unsigned max_align_vertical = 0;
    unsigned prev_id = 0;

    // align vertical
    for (unsigned i = position->y + 1; i < GRID_HEIGTH; i++) {
        if (prev_id != 0 && grid[i][position->x] != prev_id) {
            break;
        }

        prev_id = grid[i][position->x];
        max_align_vertical++;
    }

    unsigned max_align = max_align_vertical;
    unsigned max_align_horizontal = 0, max_align_horizontal_right = 0, max_align_horizontal_left = 0;
    prev_id = 0;

    // align horizontal to right
    for (int i = position->x + 1; i < GRID_WIDTH; i++) {
        if ((prev_id != 0 && grid[position->y][i] != prev_id) || grid[position->y][i] == 0) {
            break;
        }
    
        prev_id = grid[position->y][i];
        max_align_horizontal_right++;
    }

    // align horizontal to left
    for (int i = position->x - 1; i >= 0; i--) {
        if ((prev_id != 0 && grid[position->y][i] != prev_id) || grid[position->y][i] == 0) {
            break;
        }
    
        prev_id = grid[position->y][i];
        max_align_horizontal_left++;
    }

    max_align_horizontal = max_align_horizontal_left + max_align_horizontal_right;

    if (max_align < max_align_horizontal) {
        max_align = max_align_horizontal;
    }


    // align oblique first
    // align oblique second

    return max_align;
}

int get_best_column(int *max_align_per_column)
{
    for (int align_count = WIN_ALIGNMENT; align_count > 0; align_count--) {
        int count = 0;
        int column_per_align[GRID_WIDTH] = {0};
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (max_align_per_column[j] >= align_count) {
                column_per_align[count++] = j;
            }
        }

        if (count > 0) {
            if (count == 1) {
                return column_per_align[0];
            }

            return column_per_align[random_int(0, count - 1)];
        }
    }

    return 0;
}
