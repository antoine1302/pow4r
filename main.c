#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "grid.h"
#include "player.h"
#include "position.h"
#include "rule.h"
#include "robot.h"

player player_collection[] = {{PLAYER_1_ID, PLAYER_1_TOKEN}, {PLAYER_2_ID, PLAYER_2_TOKEN}};
unsigned player_amount = sizeof player_collection / sizeof (struct player);
static void get_user_input(unsigned* , unsigned (*grid)[GRID_WIDTH], unsigned*, _Bool*);
static _Bool assert_column_valid(unsigned, unsigned (*grid)[GRID_WIDTH]);
static _Bool clear_buffer(FILE *fp);
static void menu_screen(_Bool *isMultiplayer, unsigned *player_index,  unsigned (*grid)[GRID_WIDTH]);
static void loadGame(unsigned *player_index, _Bool *isMultiplayer, unsigned (*grid)[GRID_WIDTH]);
static void saveGame(unsigned *player_index, _Bool *isMultiplayer, unsigned (*grid)[GRID_WIDTH]);

int main()
{
    unsigned grid[GRID_HEIGTH][GRID_WIDTH] = {0};
    player winner;
    unsigned player_index = 0;
    _Bool multiplayer = 0;

    menu_screen(&multiplayer, &player_index, grid);

    while (1) {
        system("clear");
        display_grid(grid);
        
        unsigned column = 0;
        position last_position = {0, 0};

        if (!multiplayer && player_index == 1) {
            get_robot_input(&column, grid);
        } else {
            get_user_input(&column, grid, &player_index, &multiplayer);
        }

        for(int j = GRID_HEIGTH - 1; j >= 0; j--) {
            if (grid[j][column - 1] < 1) {
                grid[j][column - 1] = player_collection[player_index].id;
                last_position.x = column - 1;
                last_position.y = j;
                break;
            }
        }

        if (verify_winner(grid, &last_position)) {
            winner = player_collection[player_index];
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

        player_index = player_index == 0 ? 1 : 0;
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

void menu_screen(_Bool *isMultiplayer, unsigned *player_index,  unsigned (*grid)[GRID_WIDTH])
{
    unsigned menu_choice = 0;
    _Bool invalidInput = 1;

    printf("Single player (type: 1), multiplayer (type: 2), load game (type: 3)\n");
    while (invalidInput) {
        if(scanf("%u", &menu_choice) != 1) {
            printf("Invalid input\n");
        } else {
            switch (menu_choice) {
                case 1:
                    invalidInput = 0;
                    break;
                case 2:
                    *isMultiplayer = 1;
                    invalidInput = 0;
                    break;
                case 3:
                    loadGame(player_index, isMultiplayer, grid);
                    invalidInput = 0;
                    break;
                default:
                    fputs("Invalid input\n", stderr);
            }
        }

        if (!clear_buffer(stdin)) {
            fputs("Failed to clear stdin buffer\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
}

void get_user_input(unsigned *column, unsigned (*grid)[GRID_WIDTH], unsigned *player_index, _Bool *isMultiplayer)
{
    _Bool is_input_valid = 0;
    char input_char = ' ';

    do {
        printf("Player %u: ", player_collection[*player_index]);
        if (scanf("%u", column) != 1) {
            if (scanf("%c", &input_char) != 1) {
                fputs("Failed to get input. Please retry.\n", stderr);
            } else {
                switch(input_char){
                    case 's':
                    case 'S':
                        saveGame(player_index, isMultiplayer, grid);
                        printf("Game saved...\n");
                        break;
                    case 'Q':
                    case 'q':
                        printf("Bye bye...\n");
                        exit(EXIT_SUCCESS);
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
 
void saveGame(unsigned *player_index, _Bool *isMultiplayer, unsigned (*grid)[GRID_WIDTH])
{
    FILE *fp = fopen("saved_party.bin", "wb");

    if (fp == NULL)
    {
        fprintf(stderr, "Le fichier saved_party.bin n'a pas pu être ouvert\n");
        exit(EXIT_FAILURE);
    }

    if (fwrite(grid, sizeof grid[0], GRID_HEIGTH, fp) != GRID_HEIGTH)
    {
        fprintf(stderr, "Erreur lors de l'écriture du tableau\n");
        exit(EXIT_FAILURE);
    }

    if (fwrite(player_index, sizeof(unsigned), 1, fp) != 1)
    {
        fputs("Failed to save player index\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (fwrite(isMultiplayer, sizeof(_Bool), 1, fp) != 1)
    {
        fputs("Failed to multiplayer\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (fclose(fp) == EOF)
    {
        fprintf(stderr, "Erreur lors de la fermeture du flux\n");
        exit(EXIT_FAILURE);
    }
}

void loadGame(unsigned *player_index, _Bool *isMultiplayer, unsigned (*grid)[GRID_WIDTH])
{
    FILE *fp = fopen("saved_party.bin", "rb");

    if (fp == NULL)
    {
        fprintf(stderr, "Le fichier saved_party.bin n'a pas pu être ouvert\n");
        exit(EXIT_FAILURE);
    }

    if (fread(grid, sizeof grid[0], GRID_HEIGTH, fp) != GRID_HEIGTH)
    {
        fprintf(stderr, "Erreur lors de l'écriture du tableau\n");
        exit(EXIT_FAILURE);
    }

    if (fread(player_index, sizeof(unsigned), 1, fp) != 1)
    {
        fputs("Failed to save player index\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (fread(isMultiplayer, sizeof(_Bool), 1, fp) != 1)
    {
        fputs("Failed to multiplayer\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (fclose(fp) == EOF)
    {
        fprintf(stderr, "Erreur lors de la fermeture du flux\n");
        exit(EXIT_FAILURE);   
    }
}
