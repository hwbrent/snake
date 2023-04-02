#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include "../../game/src/game.h"
#include "../../food/src/food.h"
#include "../../screen/src/screen.h"
#include "snake.h"

void init_snake() {
    snake.length = INITIAL_LENGTH;

    // Initialise memory for snake segments.
    snake.rows = malloc(snake.length * sizeof *snake.rows);
    snake.cols = malloc(snake.length * sizeof *snake.rows);

    // Initialise head of snake to be in centre of screen.
    snake.rows[0] = screen.rows / 2;
    snake.cols[0] = screen.cols / 2;

    // Print the snake head.
    mvaddch(snake.rows[0], snake.cols[0], SNAKE);

    // Initialise the snake direction of movement (left horizontal).
    snake.direction[0] = 0;
    snake.direction[1] = -1;
}

void terminate_snake() {
    free(snake.rows);
    free(snake.cols);
}

void move_snake() {
    // The overall idea is that the head of the snake is moved according to
    // 'snake.direction', and each other segment takes the previous
    // position of its preceding segment.

    int prev[2];

    bool ate_food = false;

    // Move the head of the snake
    prev[0] = snake.rows[0];
    prev[1] = snake.cols[0];
    snake.rows[0] += snake.direction[0];
    snake.cols[0] += snake.direction[1];

    // Evaluate whether the head is in a valid position.
    int c = mvinch(snake.rows[0], snake.cols[0]);
    if (c == BORDER || c == SNAKE) {
        game_should_continue = false;
        return;
    } else if (c == FOOD) {
        // increment snake length
        ate_food = true;
    }

    // Print the head.
    mvaddch(snake.rows[0], snake.cols[0], SNAKE);

    // Update each segment with the position of its
    // preceding segment.
    for (int i = 1; i < snake.length; i++) {
        int temp_row = snake.rows[i];
        int temp_col = snake.cols[i];

        snake.rows[i] = prev[0];
        snake.cols[i] = prev[1];

        prev[0] = temp_row;
        prev[1] = temp_col;
    }

    if (ate_food) {
        // Add a new segment to the snake.

        snake.length++;

        snake.rows = realloc(snake.rows, snake.length * sizeof *snake.rows);
        snake.cols = realloc(snake.cols, snake.length * sizeof *snake.cols);

        snake.rows[snake.length-1] = prev[0];
        snake.cols[snake.length-1] = prev[1];

        // Place the food again.
        place_food();
    } else {
        mvaddch(prev[0], prev[1], EMPTY);
    }

    refresh();
}
