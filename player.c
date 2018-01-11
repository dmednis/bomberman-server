#include "player.h"


PlayerList *create_playerlist() {
    PlayerList *list = malloc(sizeof(PlayerList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


Player *create_player(int id, char* name, Socket* socket) {
    Player *player = malloc(sizeof(Player));
    player->id = id;
    player->name = name;
    player->socket = socket;
    player->last_ping = time(NULL);

    player->next = NULL;
    player->prev = NULL;

    return player;
}


Player *add_player(Game* game, Player *player) {
    PlayerList *list = game->players;

    if (list->tail != NULL) {
        list->tail->next = player;
        player->prev = list->tail;
        list->tail = player;
    } else {
        list->head = player;
        list->tail = player;
    }

    list->size++;

    return player;
}


void remove_player(Game* game, Player *player) {
    PlayerList *list = game->players;

    if (player->prev) {
        player->prev->next = player->next;
    } else {
        list->head = player->next;
    }

    if (player->next) {
        player->next->prev = player->prev;
    } else {
        list->tail = player->prev;
    }

    list->size--;

    free(player);
};