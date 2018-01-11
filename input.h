//
// Created by Dāvis Mednis on 07/01/2018.
//

#ifndef BOMBERMAN_SERVER_INPUT_H
#define BOMBERMAN_SERVER_INPUT_H

pthread_t create_input_reader_thread();

void input_reader(void*);

#endif //BOMBERMAN_SERVER_INPUT_H
