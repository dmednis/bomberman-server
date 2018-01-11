#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "net.h"


void* input_reader(void* arg) {
    char input[64];

    while (strcmp(gets(input), "exit") != 0) {
        printf("Server input: %s \n", input);
        if (strcmp(input, "stop") == 0) {
            stop_server();
        }
        bzero(input, 64);
    }
    return NULL;
}


pthread_t create_input_reader_thread() {
    pthread_t input_reader_thread;
    pthread_create(&input_reader_thread, NULL, input_reader, NULL);
    return input_reader_thread;
}


