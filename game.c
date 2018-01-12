#import "game.h"

Game* game;


/**
 *
 * @return
 */
Game *create_game() {
    game = malloc(sizeof(Game));

    game->player_id_seq = 1;
    game->state = GS_LOBBY;
    game->players = create_playerlist();

    return game;
}


/**
 *
 * @return
 */
PlayerList *create_playerlist() {
    PlayerList *list = malloc(sizeof(PlayerList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


/**
 *
 * @param id
 * @param name
 * @param socket
 * @return
 */
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


/**
 *
 * @param game
 * @param player
 * @return
 */
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


/**
 *
 * @param game
 * @param player
 */
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

    free(player->name);
    free(player);
};


/**
 *
 * @param socket
 * @return
 */
void *connection_handler(void *socket) {
    //Get the socket descriptor
    Socket *sock = (Socket *) socket;
    int socket_d = *sock->socket;

    ssize_t read_size;
    unsigned char *payload = malloc(200);

    Player *player = NULL;

    while ((read_size = recv(socket_d, payload, 200, 0)) > 0) {
        char packet_code = payload[0];
        switch (packet_code) {
            case PT_JOIN_REQ: {
                handle_join_request(sock, player, payload);
                break;
            }
            case PT_KEEPALIVE: {
                handle_keepalive_packet(sock, player, payload);
                break;
            }
            case PT_PLAYER_READY: {
                handle_player_ready_packet(sock, player, payload);
                break;
            }
            case PT_PLAYER_INPUT: {
                handle_player_input_packet(sock, player, payload);
                break;
            }
            case PT_DISCONNECT: {
                handle_player_disconnect_packet(sock, player, payload);
                break;
            }
            default: {
                handle_join_request(sock, player, payload);
                break;
            }
        }

        bzero(payload, 200);
    }

    if (read_size == 0) {
        remove_player(game, player);
        delete_socket(sock);
        send_lobby_status(game);
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    delete_socket(sock);

    return 0;
}


/**
 *
 * @param payload
 * @return
 */
char *parse_join_packet(unsigned char *payload) {
    char* name = malloc(24);
    memmove(name, payload + 1, 23);
    name[23] = 0;
    puts(name);
    return name;
}


/**
 *
 * @param socket
 * @param player
 * @param payload
 */
void handle_join_request(Socket *socket, Player *player, unsigned char *payload) {
    if (player != NULL) {
        send_join_response(socket, NULL, 3);
    } else if (game->state != GS_LOBBY) {
        send_join_response(socket, NULL, 1);
    } else if (game->player_id_seq == 255) {
        send_join_response(socket, NULL, 2);
    } else {
        char* name = parse_join_packet(payload);
        if (name != NULL) {
            player = create_player(game->player_id_seq++, name, socket);
            add_player(game, player);
        }
        send_join_response(socket, player, 0);
        send_lobby_status(game);
    }

}


/**
 *
 * @param socket
 * @param player
 * @param code
 */
void send_join_response(Socket *socket, Player *player, int code) {
    int player_id;
    if (player != NULL) {
        player_id = player->id;
    } else {
        player_id = 0;
    }
    unsigned char response[3] = {2, (unsigned char)code, (unsigned char)player_id};
    write(*socket->socket, response, 3);
}



/**
 *
 * @param socket
 * @param player
 * @param code
 */
void send_lobby_status(Game *game) {
    unsigned char* message = malloc((size_t)2 + (game->players->size * 25));

    int offset = 0;

    memset(message + offset, 7, 1);
    offset += 1;
    memset(message + offset, game->players->size, 1);
    offset += 1;

    Player* player = game->players->head;

    while (player) {
        memset(message + offset, player->id, 1);
        offset += 1;
        memmove(message + offset, player->name, 23);
        offset += 23;
        memset(message + offset, player->state, 1);
        offset +=1;
        player = player->next;
    }

    broadcast(message, offset);
    free(message);
}


/**
 *
 * @param socket
 * @param player
 * @param payload
 */
void handle_keepalive_packet(Socket *socket, Player *player, unsigned char *payload) {

}


/**
 *
 * @param socket
 * @param player
 * @param payload
 */
void handle_player_ready_packet(Socket *socket, Player *player, unsigned char *payload) {

}


/**
 *
 * @param socket
 * @param player
 * @param payload
 */
void handle_player_input_packet(Socket *socket, Player *player, unsigned char *payload) {

}


/**
 *
 * @param socket
 * @param player
 * @param payload
 */
void handle_player_disconnect_packet(Socket *socket, Player *player, unsigned char *payload) {

}