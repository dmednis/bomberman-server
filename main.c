#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include<pthread.h> //for threading , link with lpthread
#include <signal.h>

#include "net.h"
#include "input.h"

void signal_handler(int sig)
{
//    stop_server();
}

void create_signal_handler() {

    printf("pid: %d\n", getpid());

    struct sigaction sa;
    sa.sa_handler = &signal_handler;
    sigaction(SIGINT, &sa, NULL);
}

int main(int argc , char *argv[])
{
    create_signal_handler();
    create_input_reader_thread();

    int listen_socket = create_server();
    bind_connection_handler(listen_socket);

    return 0;
}
