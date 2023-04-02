#ifndef SCREEN_H_   /* Include guard */
#define SCREEN_H_

struct screen {
    int rows;
    int cols;
} screen;

void init_screen();

void terminate_screen();

#endif // SCREEN_H_