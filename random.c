#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "random.h"

static double get_random();

int random_int(int min, int max)
{
    time_t t;

    if (time(&t) == (time_t)-1) {
        fputs("Unable to generate Epoch\n", stderr);
        exit(EXIT_FAILURE);
    }

    srand((unsigned)t);

    return get_random() * (max - min + 1) + min;
}


double get_random()
{
    return rand() / (RAND_MAX  + 1.);
}
