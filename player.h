#ifndef PLAYER_H
#define PLAYER_H

#define NO_TOKEN (' ')
#define PLAYER_1_TOKEN ('X')
#define PLAYER_2_TOKEN ('O')

#define PLAYER_1_ID (100)
#define PLAYER_2_ID (200)

typedef struct player 
{
    unsigned int id;
    char token_symbol;
} player;

extern player player_collection[];
extern unsigned player_amount;

#endif
