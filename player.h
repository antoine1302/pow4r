#ifndef PLAYER_H
#define PLAYER_H

#define NO_TOKEN (' ')
#define PLAYER_1_TOKEN ('X')
#define PLAYER_2_TOKEN ('O')

struct player 
{
    unsigned int id;
    char token_symbol;
};

extern struct player player_collection[];
extern unsigned player_amount;

#endif
