
#include <stdio.h>
#include <stdio.h>
#include "grid.h"

#define GRID_WIDTH 7
#define GRID_HEIGTH 6

static void display_header();
static void display_line();

void display_grid()
{
    display_header();

    for (unsigned i = 0; i < GRID_HEIGTH; i++) {

        display_line();

        fputs("|", stdout);

        for (unsigned j = 0; j < GRID_WIDTH; j++) {
            fputs("   |", stdout);    
        }

        fputs("\n", stdout);
    }

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

