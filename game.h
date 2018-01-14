#ifndef BOMBERMAN_SERVER_GAME_H
#define BOMBERMAN_SERVER_GAME_H

#include "libs.h"
#include "net.h"


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

enum PlayerState {
    PS_UNREADY = 0,
    PS_PREREADY = 1,
    PS_READY = 2,
};

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

typedef struct {
    int player_id_seq;
    int state;
    PlayerList* players;
} Game;

PlayerList *create_playerlist();

Player *create_player(int, char*, Socket*);

Player *add_player(Game*, Player*);

void remove_player(Game*, Player*);

Game* create_game();

void *connection_handler(void *);

Player * handle_join_request(Socket *socket, Player *player, unsigned char *payload);

void send_join_response(Socket *socket, Player *player, int code);

void send_lobby_status(Game *game);

void handle_keepalive_packet(Socket *socket, Player *player, unsigned char *payload);

void handle_player_ready_packet(Socket *socket, Player *player, unsigned char *payload);

void check_player_readiness();

void start_pregame();

void start_game();

void send_game_start();

void send_objects();

void send_map_update();

void handle_player_input_packet(Socket *socket, Player *player, unsigned char *payload);

void handle_player_disconnect_packet(Socket *socket, Player *player, unsigned char *payload);

#endif //BOMBERMAN_SERVER_GAME_H
