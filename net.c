#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include <assert.h>

#include "net.h"

int listen_socket;
SocketList *socketlist;

SocketList *create_socketlist() {
    SocketList *list = malloc(sizeof(SocketList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


Socket *create_socket(int *socket_d, pthread_t *thread) {
    Socket *socket = malloc(sizeof(Socket));
    socket->socket = socket_d;
    socket->thread = thread;
    socket->next = NULL;
    socket->prev = NULL;

    return socket;
}

Socket *put_socket(SocketList *list, Socket *socket) {
    if (list->tail != NULL) {
        list->tail->next = socket;
        socket->prev = list->tail;
        list->tail = socket;
    } else {
        list->head = socket;
        list->tail = socket;
    }

    list->size++;

    return socket;
}

void delete_socket(SocketList *list, Socket *socket) {

    pthread_kill((pthread_t)socket->thread, SIGTERM);
    shutdown(*socket->socket, SHUT_RDWR);
    close(*socket->socket);

    if (socket->prev) {
        socket->prev->next = socket->next;
    } else {
        list->head = socket->next;
    }

    if (socket->next) {
        socket->next->prev = socket->prev;
    } else {
        list->tail = socket->prev;
    }

    list->size--;

    free(socket->socket);
    free(socket->thread);
    free(socket);
};

int create_server() {
    socketlist = create_socketlist();

    struct sockaddr_in server;

    //Create socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    //Enables port reuse after program ends
    const int optVal = 1;
    const socklen_t optLen = sizeof(optVal);
    int rtn = setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, (void *) &optVal, optLen);
    assert(rtn == 0);

    //Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        puts("bind failed");
        exit(-1);
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    listen_socket = socket_desc;

    return socket_desc;
}


void stop_server() {
    Socket *curr = socketlist->head;

    while (curr) {
        delete_socket(socketlist, curr);
        curr = socketlist->head;
    }
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket) {
    //Get the socket descriptor
    Socket *sock = (Socket *) socket;
    int socket_d = *sock->socket;
    printf("%d\n", socket_d);
    int read_size;
    char *client_message[1000];

    put_socket(socketlist, sock);

    //Receive a message from client
    while ((read_size = recv(socket_d, client_message, 2000, 0)) > 0) {
        //Send the message back to client
        puts(client_message);
        char response[3] = {'2', '0', '1'};
        write(socket_d, response, 3);
        bzero(client_message, 1000);
    }

    if (read_size == 0) {

        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    delete_socket(socketlist, sock);

    return 0;
}


int the_socket;

int bind_connection_handler(int socket) {
    int new_socket, c, *new_sock;
    struct sockaddr_in client;
    char *message;

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket, (struct sockaddr *) &client, (socklen_t *) &c))) {
        if (new_socket == -1) {
            perror("accept failed");
        }

        the_socket = new_socket;
        puts("Connection accepted");
        //Reply to the client

        pthread_t* connection_thread = malloc(sizeof(pthread_t));
        new_sock = malloc(1);
        *new_sock = new_socket;

        Socket* socket = create_socket(new_sock, connection_thread);

        if (pthread_create(connection_thread, NULL, connection_handler, (void *) socket) < 0) {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }

    if (new_socket < 0) {
        perror("accept failed");
        return 1;
    }
}


void broadcast(char *message) {
    if (send(the_socket, message, strlen(message), 0) < 0) {
        puts("Send failed");
    }
}


void net_error_handler() {

}


