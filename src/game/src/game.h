#ifndef GAME_H_   /* Include guard */
#define GAME_H_

#include <stdbool.h>
#include <pthread.h>

#define BORDER '#'
#define SNAKE 'S'
#define FOOD 'F'
#define EMPTY ' '

bool DEBUG = false;

bool game_should_continue = true;

int pressed_key;
pthread_t ptid;

char* get_direction();

bool is_direction(char* direction);

void *getch_thread_fn(void *vargp);

int msleep(long msec);

void init_game();

void run_game();

void terminate_game();

#endif // GAME_H_