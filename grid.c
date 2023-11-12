
#include <stdio.h>
#include <stdio.h>
#include "grid.h"
#include "player.h"

static void display_header();
static void display_line();

extern struct player player_collection[];
extern unsigned player_amount;

void display_grid(unsigned (*grid)[GRID_WIDTH])
{
    display_header();

    for (unsigned i = 0; i < GRID_HEIGTH; i++) {

        display_line();

        fputs("|", stdout);

        for (unsigned j = 0; j < GRID_WIDTH; j++) {
            if (grid[i][j] > 0) {
                unsigned int player_number = grid[i][j];
                for (unsigned i = 0; i < player_amount; i++) {
                    if(player_number == player_collection[i].id) {
                        printf(" %c |", player_collection[i].token_symbol);
                        break;
                    }
                }
            } else {
                fputs("   |", stdout);
            }
        }

        fputs("\n", stdout);
    }

    display_line();

    display_header();
}

void display_header()
{
    for (unsigned i = 0; i < GRID_WIDTH; i++ ) {
        printf("  %d ", i + 1);
    }
    
    fputs("\n", stdout);
}

void display_line()
{
    fputs("+", stdout);

    for (unsigned i = 0; i < GRID_WIDTH; i++ ) {
        fputs("---+", stdout);
    }

    fputs("\n", stdout);
}

