#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "player.h"

struct player player_collection[] = {{1, PLAYER_1_TOKEN}, {2, PLAYER_2_TOKEN}};
unsigned player_amount = sizeof player_collection / sizeof (struct player);
static _Bool is_column_valid(unsigned, unsigned (*grid)[GRID_WIDTH]);

int main()
{
    unsigned grid[GRID_HEIGTH][GRID_WIDTH] = {0};

    while (1) {
        for (size_t i = 0; i < player_amount; i++) {

            display_grid(grid);
            printf("Player %u: ", player_collection[i].id);
            system("clear");
            _Bool is_input_valid = 0;
            unsigned column = 0;

            do {
                unsigned input = scanf("%u", &column);

                if(input < 1) {
                    puts("Failed to get column. Please retry.");
                    // clear buffer
                    continue;
                }

                if(!is_column_valid(input, grid)) {
                    continue;
                }

                is_input_valid = 1;

            } while(!is_input_valid);

            for(int j = GRID_HEIGTH - 1; j >= 0; j--) {
                if (grid[j][column - 1] < 1) {
                    grid[j][column - 1] = player_collection[i].id;
                    break;
                }
            }

            // check endgame
        }
    }

    return EXIT_SUCCESS;
}

_Bool is_column_valid(unsigned column, unsigned (*grid)[GRID_WIDTH])
{
    // check for out of range column
    if(column < 1 || column > GRID_WIDTH) {
        puts("Out of range value");
        return 0;
    }
    // check if the column is already full
    if(grid[GRID_HEIGTH -1][GRID_WIDTH -1] > 0) {
        puts("Column already full");
        return 0;
    }

    return 1;
}
 