//
// Created by Dāvis Mednis on 07/01/2018.
//

#ifndef BOMBERMAN_SERVER_PLAYER_H
#define BOMBERMAN_SERVER_PLAYER_H

#include <ntsid.h>

typedef struct {
    int id;
    int state;
    char* name;
    time_t last_ping;
} Player;

typedef struct {
    Player **players;
    int size;
    int length;
} PlayerList;

PlayerList *create_playerlist(int);

#endif //BOMBERMAN_SERVER_PLAYER_H
