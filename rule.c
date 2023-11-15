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
    for (int i = position->y; i < GRID_HEIGTH; i++) {
        if (grid[i][position->x] == player_id) {
            aligned_count++;
            if(aligned_count >= WIN_ALIGNMENT) {
                return 1;
            }
        } else {
            aligned_count = 0;
        }
    }

    // verify oblique
    struct position start_position = { position->x, position->y };
    while (start_position.x < GRID_WIDTH -1 && start_position.y < GRID_HEIGTH -1) {
        start_position.x++;
        start_position.y++;
    }

    aligned_count = 0;

    while (start_position.x >= 0 && start_position.y >= 0) {
        if(player_id == grid[start_position.x][start_position.y]) {
            aligned_count++;
            if(aligned_count >= WIN_ALIGNMENT) {
                return 1;
            }
        } else {
            aligned_count = 0;
        }
        start_position.x--;
        start_position.y--;
    }

    start_position.x = position->x;
    start_position.y = position->y;
    while (start_position.x > 0 && start_position.y < GRID_HEIGTH -1) {
        start_position.x--;
        start_position.y++;
    }

    aligned_count = 0;

    while (start_position.x < GRID_WIDTH && start_position.y >= 0) {
        if(player_id == grid[start_position.x][start_position.y]) {
            aligned_count++;
            if(aligned_count >= WIN_ALIGNMENT) {
                return 1;
            }
        } else {
            aligned_count = 0;
        }
        start_position.x++;
        start_position.y--;
    }

    return 0;
}


