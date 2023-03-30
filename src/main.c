#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include <unistd.h>
#include <sys/ioctl.h>

#include <time.h>
#include <errno.h>

#include <pthread.h>

char BORDER = '#';
char SNAKE = 'S';
char FOOD = 'F';
char EMPTY = ' ';

bool DEBUG = false;

bool game_should_continue = true;

int pressed_key;
pthread_t ptid;

struct screen {
    // The number of rows in the screen.
    int rows;

    // The number of columns in the screen.
    int cols; 

    // The total number of pixels contained in the screen.
    int total_pixels;

    // The char value that each pixel in the screen holds.
    // An array of length 'screen.total_pixels'.
    char* pixels;
} screen;

struct snake {
    int length;

    // The row and column numbers of each segment of the snake.
    int* segments_rows;
    int* segments_cols;

    // Whether a new segment should be added to the snake.
    bool should_add_seg;

    int direction[2];
} snake;

struct food {
    int row;
    int col;
    int value;
} food;


/* -------------------------------------------------- */

// Row and Column TO Index
int rctoi(int row, int col) {
    return (screen.cols * row) + col;
}

int itorc[2] = { -1, -1 };
// Index TO Row and Column
void set_itorc(int i) {
    itorc[0] = i / screen.rows;
    itorc[1] = i % screen.cols;
}

void set_pixel(int i, char value) {
    screen.pixels[i] = value;
}

char get_pixel(int i) {
    return screen.pixels[i];
}

void init_screen() {
    system("clear");

    if (!DEBUG) {
        initscr();
        getmaxyx(stdscr, screen.rows , screen.cols); // Sets screen.rows and screen.cols
        cbreak();
        keypad(stdscr, TRUE);
        noecho();
    } else {
        // See https://stackoverflow.com/a/1022961
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        screen.rows = w.ws_row;
        screen.cols = w.ws_col;
    }

    screen.total_pixels = screen.rows * screen.cols;

    screen.pixels = (char*)malloc(screen.total_pixels * sizeof(char));
    for (int i = 0; i < screen.rows; i++) {
        bool is_top_bottom_border =
            (i == 0) || (i == screen.rows-1);
        for (int j = 0; j < screen.cols; j++) {
            bool is_left_right_border = 
                (j == 0) || (j == screen.cols-1);
            bool is_border = (is_top_bottom_border || is_left_right_border);

            set_pixel(rctoi(i,j), is_border ? BORDER : EMPTY);
        }
    }
}

void terminate_screen() {
    endwin();
    free(screen.pixels);
}

void print_screen() {
    if (DEBUG) {
        return;
    }

    for (int i = 0; i < screen.rows; i++) {
        for (int j = 0; j < screen.cols; j++) {
            int pixel_index = rctoi(i,j);
            char pixel = get_pixel(pixel_index);
            mvaddch(i, j, pixel);
        }
    }
    refresh();
}

/* -------------------------------------------------- */

void update_segment(int seg_i, int new_row, int new_col, int old_row, int old_col) {
    snake.segments_rows[seg_i] = new_row;
    snake.segments_cols[seg_i] = new_col;
    set_pixel(rctoi(new_row, new_col), SNAKE);

    if (old_row > -1 && old_col > -1) {
        set_pixel(rctoi(new_row, new_col), EMPTY);
    }
}

void init_snake() {
    snake.length = 1;

    // Initial size of 1 because snake starts with only one segment.
    snake.segments_rows = (int*)malloc(snake.length * sizeof(int));
    snake.segments_cols = (int*)malloc(snake.length * sizeof(int));

    // Initialise snake head to be centre of screen.
    update_segment(0, screen.rows/2, screen.cols/2, -1, -1);

    if (DEBUG) {
        printf("Initial snake head position -> [%d, %d]\n", snake.segments_rows[0], snake.segments_cols[0]);
    }

    snake.should_add_seg = false;

    snake.direction[0] = 0;
    snake.direction[1] = -1;
}

void terminate_snake() {
    free(snake.segments_rows);
    free(snake.segments_cols);
}

void place_food() {
    int empty_count = 0;
    int* empty_pixels = malloc(0);

    for (int i = 1; i < screen.rows-1; i++) {
        for (int j = 1; j < screen.cols-1; j++) {
            int index = rctoi(i,j);
            if (get_pixel(index) != EMPTY) {
                continue;
            }

            empty_count++;

            // Increment the size of the array.
            empty_pixels = (int*)realloc(empty_pixels, empty_count * sizeof(int));

            empty_pixels[empty_count-1] = index;
        }
    }

    // Pick a random value from empty_pixels
    int random_index = rand() % (empty_count - 1);

    if (food.row > 0 && food.col > 0) {
        set_pixel(rctoi(food.row, food.col), EMPTY);
    }

    set_itorc(random_index);
    food.row = itorc[0];
    food.col = itorc[1];

    set_pixel(random_index, FOOD);

    free(empty_pixels);
}

void move_snake() {
    int prev_row;
    int prev_col;

    int current_row;
    int current_col;

    for (int i = 0; i < snake.length; i++) {
        current_row = snake.segments_rows[i];
        current_col = snake.segments_cols[i];

        if (i == 0) {
            prev_row = current_row;
            prev_col = current_col;

            int new_row = current_row+snake.direction[0];
            int new_col = current_col+snake.direction[1];

            // If the head of the snake hits the food.
            if (get_pixel(rctoi(new_row, new_col)) == FOOD) {
                // printf("\nhit food innit\n");
                snake.should_add_seg = true;
            }

            update_segment(
                i,
                new_row,
                new_col,
                -1,
                -1
            );

        } else {
            int temp_row = current_row;
            int temp_col = current_col;

            update_segment(i, prev_row, prev_col, -1, -1);

            prev_row = temp_row;
            prev_col = temp_col;
        }
    }

    if (snake.should_add_seg) {
        snake.should_add_seg = false;

        snake.length++;

        snake.segments_rows = (int*)realloc(snake.segments_rows, snake.length * sizeof(int));
        snake.segments_cols = (int*)realloc(snake.segments_cols, snake.length * sizeof(int));

        update_segment(snake.length-1, prev_row, prev_col, -1, -1);

        place_food();
    } else {
        set_pixel(rctoi(prev_row, prev_col), EMPTY);
    }
}

void eval_snake_pos() {
    // Scenarios:
    // - Snake head is touching border                 -> game over
    // - Snake head is touching another part of itself -> game over
    // - Snake head is touching food                   -> snake gets longer

    int head_row = snake.segments_rows[0];
    int head_col = snake.segments_cols[0];

    bool is_on_border =
        (head_row == 0 || head_row == screen.rows - 1) ||
        (head_col == 0 || head_col == screen.cols - 1);

    if (is_on_border) {
        game_should_continue = false;
        return;
    }

    // Check if snake is eating itself.
    for (int i = 1; i < snake.length; i++) {
        int seg_row = snake.segments_rows[i];
        int seg_col = snake.segments_cols[i];

        if (seg_row == head_row && seg_col == head_col) {
            game_should_continue = false;
            return;
        }
    }
}

/* -------------------------------------------------- */

void init_food() {
    srand(time(NULL));
    place_food();
}

void terminate_food() {}

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
    if (msec < 0)
    { errno = EINVAL; return -1; }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do { res = nanosleep(&ts, &ts); } while (res && errno == EINTR);
    return res;
}

/* -------------------------------------------------- */

void run_game() {
    while (game_should_continue) {
        print_screen();
        move_snake();
        eval_snake_pos();
        msleep(125);
    }

    // getch();
}

/* -------------------------------------------------- */

int main(int argc, char **argv) {
    init_game();
    run_game();
    terminate_game();
    return 0;
}
