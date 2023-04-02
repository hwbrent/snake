#ifndef FOO_H_   /* Include guard */
#define FOO_H_

struct food {
    int row;
    int col;
} food;

void init_food();

void terminate_food();

void place_food();

#endif // FOO_H_