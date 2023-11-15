#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"
#include "player.h"
#include "position.h"
#include "rule.h"

struct player player_collection[] = {{100, PLAYER_1_TOKEN}, {200, PLAYER_2_TOKEN}};
unsigned player_amount = sizeof player_collection / sizeof (struct player);
static _Bool assert_column_valid(unsigned, unsigned (*grid)[GRID_WIDTH]);
static _Bool clear_buffer(FILE *fp);

int main()
{
    unsigned grid[GRID_HEIGTH][GRID_WIDTH] = {0};
    struct player winner;

    while (1) {
        for (size_t i = 0; i < player_amount; i++) {

            system("clear");
            display_grid(grid);
            _Bool is_input_valid = 0;
            unsigned column = 0;
            struct position last_position = {0, 0};

            do {

                printf("Player %u: ", player_collection[i].id);
                unsigned input = scanf("%u", &column);

                if (!clear_buffer(stdin)) {
                    fputs("Failed to clear stdin buffer\n", stderr);
                    return EXIT_FAILURE;
                }

                if (input != 1) {
                    fputs("Failed to get column. Please retry.\n", stderr);
                    continue;
                }

                if (!assert_column_valid(column, grid)) {
                    continue;
                }

                is_input_valid = 1;

            } while(!is_input_valid);

            for(int j = GRID_HEIGTH - 1; j >= 0; j--) {
                if (grid[j][column - 1] < 1) {
                    grid[j][column - 1] = player_collection[i].id;
                    last_position.x = column - 1;
                    last_position.y = j;
                    break;
                }
            }

            if (verify_end(grid, &last_position)) {
                winner = player_collection[i];
                system("clear");
                display_grid(grid);
                goto winner;
            }
        }
    }

winner:
    printf("Player %u is the winner\n", winner.id);

    return EXIT_SUCCESS;
}

_Bool assert_column_valid(unsigned column, unsigned (*grid)[GRID_WIDTH])
{
    // check for out of range column
    if(column < 1 || column > GRID_WIDTH) {
        fputs("Out of range value\n", stderr);
        return 0;
    }
    // check if the column is already full
    if(grid[0][column - 1] > 0) {
        fputs("Column already full\n", stderr);
        return 0;
    }

    return 1;
}

_Bool clear_buffer(FILE *fp)
{
    int c = 0;

    do {
        c = fgetc(fp);
    } while(c != '\n' && c != EOF);

    return ferror(fp) > 0 ? 0 : 1;
}
 