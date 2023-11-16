#ifndef RULE_H
#define RULE_H

#include <stdlib.h>
#include "grid.h"
#include "position.h"

#define WIN_ALIGNMENT 4

extern _Bool verify_winner(unsigned (*grid)[GRID_WIDTH], struct position *position);
extern _Bool verify_draw(unsigned (*grid)[GRID_WIDTH]);

#endif
