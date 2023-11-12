#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "player.h"

struct player player_collection[] = {{1, PLAYER_1_TOKEN}, {2, PLAYER_2_TOKEN}};
unsigned player_amount = sizeof player_collection / sizeof (struct player);

int main()
{
    unsigned grid[GRID_HEIGTH][GRID_WIDTH] = {0};

    while (1) {
        for (size_t i = 0; i < player_amount; i++) {

            system("clear");

            display_grid(grid);
            printf("Player %u: ", player_collection[i].id);
            unsigned column = 0;
            int ret = scanf("%u", &column);

            // control overflow column 

            if(ret < 1) {
                puts("Failed to get column");
                return EXIT_FAILURE;
            }

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
 