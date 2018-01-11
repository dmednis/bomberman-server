//
// Created by Dāvis Mednis on 07/01/2018.
//

#ifndef BOMBERMAN_SERVER_PLAYER_H
#define BOMBERMAN_SERVER_PLAYER_H

#include "libs.h"
#include "game.h"
#include "net.h"

typedef struct Player{
    int id;
    int state;
    char* name;
    time_t last_ping;

    Socket* socket;

    struct Player *next;
    struct Player *prev;
} Player;

typedef struct {
    Player* head;
    Player* tail;
    int size;
} PlayerList;

PlayerList *create_playerlist();

Player *create_player(int, char*, Socket*);

Player *add_player(Game*, Player*);

void remove_player(Game*, Player*);

#endif //BOMBERMAN_SERVER_PLAYER_H
