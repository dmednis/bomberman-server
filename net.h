#ifndef BOMBERMAN_SERVER_NET_H
#define BOMBERMAN_SERVER_NET_H

typedef struct Socket {
    struct Socket* next;
    struct Socket* prev;
    int* socket;
    pthread_t* thread;
} Socket;

typedef struct SocketList {
    Socket* head;
    Socket* tail;
    int size;
} SocketList;

Socket* create_socket(int*, pthread_t*);

SocketList *create_socketlist();

Socket* put_socket(SocketList*, Socket*);

void delete_socket(SocketList*, Socket*);

int create_server();

void stop_server();

int bind_connection_handler(int);

void *connection_handler(void *);

void broadcast(char*);

void net_error_handler();

#endif //BOMBERMAN_SERVER_NET_H
