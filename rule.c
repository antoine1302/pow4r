#include <stdio.h>
#include <stdlib.h>
#include "rule.h"
#include "grid.h"
#include "position.h"

_Bool verify_end(unsigned (*grid)[GRID_WIDTH], struct position *position)
{
    unsigned player_id = grid[position->y][position->x];
    unsigned aligned_count = 0;

    // verify horizontally
    for (unsigned i = 0; i < GRID_WIDTH; i++) {
        if (grid[position->y][i] == player_id) {
            aligned_count++;
            if(aligned_count >= WIN_ALIGNMENT) {
                return 1;
            }
        } else {
            aligned_count = 0;
        }
    }

    // verify vertically
    for (unsigned i = position->y; i < GRID_HEIGTH; i++) {
        if (grid[i][position->x] == player_id) {
            aligned_count++;
            if(aligned_count >= WIN_ALIGNMENT) {
                return 1;
            }
        } else {
            aligned_count = 0;
        }
    }


    return 0;
}


