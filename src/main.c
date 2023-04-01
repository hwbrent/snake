#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include <unistd.h>
#include <sys/ioctl.h>

#include <time.h>
#include <errno.h>

#include <pthread.h>

#define SNAKE_INITIAL_LENGTH 1

#define BORDER '#'
#define SNAKE 'S'
#define FOOD 'F'
#define EMPTY ' '

bool DEBUG = false;

bool game_should_continue = true;

int pressed_key;
pthread_t ptid;

struct screen {
    int rows;
    int cols;
    int total_pixels;
} screen;

struct snake {
    int length;
    int* head[2];
    int* rows;
    int* cols;
    int direction[2];
} snake;

struct food {
    int row;
    int col;
} food;

/* -------------------------------------------------- */

void init_screen() {
    system("clear");

    if (!DEBUG) {
        initscr();
        getmaxyx(stdscr, screen.rows , screen.cols); // Sets screen.rows and screen.cols
        cbreak();
        keypad(stdscr, TRUE);
        noecho();
        curs_set(0);
    } else {
        // See https://stackoverflow.com/a/1022961
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        screen.rows = w.ws_row;
        screen.cols = w.ws_col;
    }

    for (int i = 0; i < screen.rows; i++) {

        bool is_top_bottom_border =
            (i == 0) || (i == screen.rows-1);

        for (int j = 0; j < screen.cols; j++) {

            bool is_left_right_border = 
                (j == 0) || (j == screen.cols-1);

            bool is_border = (is_top_bottom_border || is_left_right_border);

            mvaddch(i, j, is_border ? BORDER : EMPTY);
        }
    }
}

void terminate_screen() {
    endwin();
}

/* -------------------------------------------------- */

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

/* -------------------------------------------------- */

void init_snake() {
    snake.length = SNAKE_INITIAL_LENGTH;

    snake.rows = malloc(snake.length * sizeof *snake.rows);
    snake.rows[0] = screen.rows / 2;

    snake.cols = malloc(snake.length * sizeof *snake.rows);
    snake.cols[0] = screen.cols / 2;

    mvaddch(
        snake.rows[0],
        snake.cols[0],
        SNAKE
    );

    snake.direction[0] = 0;
    snake.direction[1] = -1;
}

void terminate_snake() {
    free(snake.rows);
    free(snake.cols);
}

void move_snake() {
    int prev[2];

    bool ate_food = false;

    for (int i = 0; i < snake.length; i++) {
        if (i == 0) {
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

            mvaddch(snake.rows[0], snake.cols[0], SNAKE);

        } else {
            int temp_row = snake.rows[i];
            int temp_col = snake.cols[i];

            snake.rows[i] = prev[0];
            snake.cols[i] = prev[1];

            prev[0] = temp_row;
            prev[1] = temp_col;
        }
    }

    if (ate_food) {
        snake.length++;

        snake.rows = realloc(snake.rows, snake.length * sizeof *snake.rows);
        snake.cols = realloc(snake.cols, snake.length * sizeof *snake.cols);

        snake.rows[snake.length-1] = prev[0];
        snake.cols[snake.length-1] = prev[1];

        place_food();
    } else {
        mvaddch(prev[0], prev[1], EMPTY);
    }

    refresh();
}

/* -------------------------------------------------- */

char* get_direction() {
    if (snake.direction[0] == -1 && snake.direction[1] == 0) {
        return "up";
    } else if (snake.direction[0] == 1 && snake.direction[1] == 0) {
        return "down";
    } else if (snake.direction[0] == 0 && snake.direction[1] == -1) {
        return "left";
    } else /* if (snake.direction[0] == 0 && snake.direction[1] == 1) */ {
        return "right";
    }
}

bool is_direction(char* direction) {
    return direction == get_direction();
}

void *getch_thread_fn(void *vargp) {
    while (true) {
        pressed_key = getch();
        switch (pressed_key) {
            case 'q':
                game_should_continue = false;
                break;

            case 'w':
            case KEY_UP:
                if (!is_direction("down")) {
                    snake.direction[0] = -1;
                    snake.direction[1] = 0;
                }
                break;

            case 'a':
            case KEY_LEFT:
                if (!is_direction("right")) {
                    snake.direction[0] = 0;
                    snake.direction[1] = -1;
                }
                break;

            case 's':
            case KEY_DOWN:
                if (!is_direction("up")) {
                    snake.direction[0] = 1;
                    snake.direction[1] = 0;
                }
                break;

            case 'd':
            case KEY_RIGHT:
                if (!is_direction("left")) {
                    snake.direction[0] = 0;
                    snake.direction[1] = 1;
                }
                break;

            default:
                break;
        }
    }
}

/* -------------------------------------------------- */

void init_game() {
    init_screen();
    init_snake();
    init_food();
    pthread_create(&ptid, NULL, &getch_thread_fn, NULL);
}

void terminate_game() {
    terminate_screen();
    terminate_snake();
    terminate_food();
    pthread_cancel(ptid);
}

/* -------------------------------------------------- */

// See https://stackoverflow.com/a/1157217
int msleep(long msec) {
    struct timespec ts;
    int res;
    if (msec < 0) { errno = EINVAL; return -1; }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do { res = nanosleep(&ts, &ts); } while (res && errno == EINTR);
    return res;
}

/* -------------------------------------------------- */

void run_game() {
    // for (int i = 0; i < 10; i++) {
    while (game_should_continue) {
        move_snake();
        msleep(125);
    }
}

/* -------------------------------------------------- */

int main(int argc, char **argv) {
    init_game();
    run_game();
    terminate_game();
    return 0;
}
