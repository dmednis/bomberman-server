#import "game.h"

Game* game;

Game* create_game() {
    Game* game = malloc(sizeof(Game));

    game->player_id_seq = 1;
    game->state = GS_LOBBY;
    game->players = create_playerlist();

    return game;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket) {
    //Get the socket descriptor
    Socket *sock = (Socket *) socket;
    int socket_d = *sock->socket;

    ssize_t read_size;
    char *payload = malloc(200);

    Player *player = NULL;

    while ((read_size = recv(socket_d, payload, 200, 0)) > 0) {
        char packet_code = payload[0];
        puts(payload);
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
                break;
            }
        }

        char response[3] = {'2', '0', '1'};
        write(socket_d, response, 3);

        bzero(payload, 200);
    }

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    delete_socket(sock);

    return 0;
}


char *parse_join_packet(char *payload) {
    return NULL;
}

void handle_join_request(Socket *socket, Player *player, char *payload) {
    if (player != NULL || game->state != GS_LOBBY) {

    } else {
        char* name = parse_join_packet(payload);
        if (name != NULL) {
            player = create_player(game->player_id_seq++, name, socket);
            add_player(game, player);
        }
    }

}

void handle_keepalive_packet(Socket *socket, Player *player, char *payload) {

}

void handle_player_ready_packet(Socket *socket, Player *player, char *payload) {

}

void handle_player_input_packet(Socket *socket, Player *player, char *payload) {

}

void handle_player_disconnect_packet(Socket *socket, Player *player, char *payload) {

}