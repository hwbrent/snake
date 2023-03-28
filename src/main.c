#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include <time.h>
#include <errno.h>

enum Chars {
    BORDER = '#',
    SNAKE = 'S',
    FOOD = 'F',
    EMPTY = ' '
};

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

    // The row and column numbers of the head (first segment) of the snake.
    int* head[2];

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
    initscr();
    getmaxyx(stdscr, screen.rows , screen.cols); // Sets screen.rows and screen.cols
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    screen.total_pixels = screen.rows * screen.cols;

    screen.pixels = (char*)malloc(screen.total_pixels * sizeof(char));
    for (int i = 0; i < screen.rows; i++) {
        bool is_top_bottom_border =
            (i == 0) || (i == screen.rows-1);
        for (int j = 0; j < screen.cols; j++) {
            bool is_left_right_border = 
                (j == 0) || (j == screen.cols-1);
            bool is_border = (is_top_bottom_border || is_left_right_border);

            enum Chars pixel_char = is_border ? BORDER : EMPTY;

            set_pixel(rctoi(i,j), pixel_char);
        }
    }
}

void terminate_screen() {
    endwin();
    free(screen.pixels);
}

void print_screen() {
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

void set_segment_pos(int seg_index, int row, int col) {
    // snake.segments_rows[seg_index] = row;
    // snake.segments_rows[seg_index] = col;
}

void init_snake() {
    snake.length = 1;

    // Initial size of 1 because snake starts with only one segment.
    snake.segments_rows = (int*)malloc(sizeof(int));
    snake.segments_cols = (int*)malloc(sizeof(int));

    // Set snake head as reference to first segment of segments.
    snake.head[0] = &(snake.segments_rows[0]);
    snake.head[1] = &(snake.segments_cols[1]);

    // Initialise snake head to be centre of screen.
    *(snake.head[0]) = screen.rows / 2;
    *(snake.head[1]) = screen.cols / 2;

    // Update screen pixels with snake head
    enum Chars snake_char = SNAKE;
    set_pixel(
        rctoi(
            *(snake.head[0]),
            *(snake.head[1])
        ),
        SNAKE
    );

    snake.should_add_seg = false;

    snake.direction[0] = 0;
    snake.direction[1] = -1;
}

void terminate_snake() {
    free(snake.segments_rows);
    free(snake.segments_cols);
}

void move_snake() {
    int prev[2];

    prev[0] = *(snake.head[0]);
    prev[1] = *(snake.head[1]);

    *(snake.head[0]) += snake.direction[0];
    *(snake.head[1]) += snake.direction[1];

    set_pixel(
        rctoi(
            *(snake.head[0]),
            *(snake.head[1])
        ),
        'S'
    );

    set_pixel(
        rctoi(prev[0], prev[1]),
        ' '
    );
}

/* -------------------------------------------------- */

void init_food() {}

void terminate_food() {}

void place_food() {}

/* -------------------------------------------------- */

void init_game() {
    init_screen();
    init_snake();
    init_food();
}
void terminate_game() {
    terminate_screen();
    terminate_snake();
    terminate_food();
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
    for (int i = 0; i < 10; i++) {
        print_screen();
        move_snake();
        msleep(250);
    }

    getch();
}

/* -------------------------------------------------- */

int main(int argc, char **argv) {
    init_game();
    run_game();
    terminate_game();
    return 0;
}
