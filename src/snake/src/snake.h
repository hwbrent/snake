#ifndef SNAKE_H_   /* Include guard */
#define SNAKE_H_

#define INITIAL_LENGTH 1

struct snake {
    int length;
    int* rows;
    int* cols;
    int direction[2];
} snake;

void init_snake();

void terminate_snake();

void move_snake();

#endif // SNAKE_H_