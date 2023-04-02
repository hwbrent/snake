#include <stdlib.h>

#include "food.h"
#include "screen.h"
#include "game.h"

void place_food() {
    // Find all the spaces in which the food could be put.
    int* empty_rows = malloc(0);
    int* empty_cols = malloc(0);
    int count = 0;
    for (int i = 1; i < (screen.rows - 1); i++) {
        for (int j = 1; j < (screen.cols - 1); j++) {
            int pixel = mvinch(i,j);
            if (pixel != EMPTY) {
                continue;
            }

            count++;
            empty_rows = realloc(empty_rows, count * sizeof *empty_rows);
            empty_cols = realloc(empty_cols, count * sizeof *empty_cols);
            empty_rows[count-1] = i;
            empty_cols[count-1] = j;
        }
    }

    int random_index = rand() % count;
    food.row = empty_rows[random_index];
    food.col = empty_cols[random_index];
    mvaddch(food.row, food.col, FOOD);
    refresh();

    free(empty_rows);
    free(empty_cols);
}

void init_food() {
    srand(time(NULL));
    place_food();
}

void terminate_food() {}
