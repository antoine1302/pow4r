
#include <stdio.h>
#include <stdio.h>
#include "grid.h"

#define GRID_WIDTH 7
#define GRID_HEIGTH 6

static void display_grid_header();

void display_grid()
{
    display_grid_header();
}

void display_grid_header()
{
    for (unsigned i = 0; i < GRID_WIDTH; i++ ) {
        printf("  %d ", i + 1);
    }
    
    printf("\n+");

    for (unsigned i = 0; i < GRID_WIDTH; i++ ) {
        printf("---+");
    }
}

