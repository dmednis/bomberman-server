//
// Created by Dāvis Mednis on 07/01/2018.
//

#include <stdlib.h>
#include "player.h"

PlayerList *create_playerlist(int initial_size) {
    PlayerList *list = malloc(sizeof(PlayerList));
    list->players = malloc(sizeof(Player *) * initial_size);
    list->size = initial_size;
    list->length = 0;
    return list;
}