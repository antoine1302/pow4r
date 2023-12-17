#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "grid.h"
#include "player.h"
#include "position.h"
#include "rule.h"
#include "robot.h"

struct player player_collection[] = {{PLAYER_1_ID, PLAYER_1_TOKEN}, {PLAYER_2_ID, PLAYER_2_TOKEN}};
unsigned player_amount = sizeof player_collection / sizeof (struct player);
static void get_user_input(unsigned*, unsigned (*grid)[GRID_WIDTH], unsigned);
static _Bool assert_column_valid(unsigned, unsigned (*grid)[GRID_WIDTH]);
static _Bool clear_buffer(FILE *fp);
static _Bool is_multiplayer();

int main()
{
    unsigned grid[GRID_HEIGTH][GRID_WIDTH] = {0};
    struct player winner;
    _Bool multiplayer = is_multiplayer();

    while (1) {
        for (size_t i = 0; i < player_amount; i++) {

            system("clear");
            display_grid(grid);
            
            unsigned column = 0;
            struct position last_position = {0, 0};

            if (!multiplayer && i == 1) {
                get_robot_input(&column, grid);
            } else {
                get_user_input(&column, grid, player_collection[i].id);
            }

            for(int j = GRID_HEIGTH - 1; j >= 0; j--) {
                if (grid[j][column - 1] < 1) {
                    grid[j][column - 1] = player_collection[i].id;
                    last_position.x = column - 1;
                    last_position.y = j;
                    break;
                }
            }

            if (verify_winner(grid, &last_position)) {
                winner = player_collection[i];
                system("clear");
                display_grid(grid);
                printf("Player %u is the winner\n", winner.id);
                return EXIT_SUCCESS;
            }

            if (verify_draw(grid)) {
                system("clear");
                display_grid(grid);
                printf("Draw\n");
                return EXIT_SUCCESS;
            }
        }
    }

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

_Bool is_multiplayer()
{
    unsigned player_count = 0;
    
    while (player_count < 1 || player_count > 2) {
        printf("Single player (type: 1) or multiplayer (type: 2)\n");
        int ret = scanf("%u", &player_count);

        if (!clear_buffer(stdin)) {
            fputs("Failed to clear stdin buffer\n", stderr);
            exit(EXIT_FAILURE);
        }

        if (ret != 1) {
            printf("Invalid input\n");
        }
    }

    return player_count == 2 ? 1 : 0;
}

void get_user_input(unsigned *column, unsigned (*grid)[GRID_WIDTH], unsigned player_id)
{
    _Bool is_input_valid = 0;
    char input_char = ' ';

    do {
        printf("Player %u: ", player_id);

        
        if (scanf("%u", column) != 1) {
            if (scanf("%c", &input_char) != 1) {
                fputs("Failed to get input. Please retry.\n", stderr);
                sleep(3);
            } else {
                switch(input_char){
                    case 's':
                    case 'S':
                        printf("you printed a char\n");
                        // save
                        break;
                    case 'Q':
                    case 'q':
                        printf("you printed a char\n");
                        // quit
                        break;
                    case 'L':
                    case 'l':
                        printf("you printed a char\n");
                        // load
                        break;
                    default:
                        printf("Invalid character (s) save, (q) quit, (l) load\n");
                }
            }
        } else {
            if (assert_column_valid(*column, grid)) {
                is_input_valid = 1;
            }
        }

        if (!clear_buffer(stdin)) {
            fputs("Failed to clear stdin buffer\n", stderr);
            exit(EXIT_FAILURE);
        }
    } while(!is_input_valid);
}
 
