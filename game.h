#ifndef BOMBERMAN_SERVER_GAME_H
#define BOMBERMAN_SERVER_GAME_H

#include "libs.h"
#include "net.h"
#include "player.h"

enum PacketType {
    PT_JOIN_REQ = 1,
    PT_JOIN_RES = 2,
    PT_KEEPALIVE = 3,
    PT_PLAYER_READY = 4,
    PT_PLAYER_INPUT = 5,
    PT_DISCONNECT = 6,
    PT_LOBBY_STATUS = 7,
    PT_GAME_START = 8,
    PT_MAP_UPDATE = 9,
    PT_OBJECTS = 10,
    PT_GAME_OVER = 11,
};

enum GameState {
    GS_LOBBY = 0,
    GS_PREGAME = 1,
    GS_GAME = 2,
    GS_GAMEOVER = 3,
};

typedef struct {
    int player_id_seq;
    int state;
    PlayerList* players;
} Game;

void *connection_handler(void *);

void handle_join_request(Socket *socket, Player *player, char *payload);

void handle_keepalive_packet(Socket *socket, Player *player, char *payload);

void handle_player_ready_packet(Socket *socket, Player *player, char *payload);

void handle_player_input_packet(Socket *socket, Player *player, char *payload);

void handle_player_disconnect_packet(Socket *socket, Player *player, char *payload);

#endif //BOMBERMAN_SERVER_GAME_H
