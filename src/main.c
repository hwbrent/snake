#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdbool.h>

// For 'msleep'
#include <time.h>
#include <errno.h>

struct screen {
    int rows;
    int cols;
    int total_pixel_count;
    char* pixels;
};
struct screen screen;

struct snake {
    int* row_coords;
    int* col_coords;
    int direction[2];
};

struct snake snake;

// Converts a row,col coordinate to an absolute screen coordinate.
// e.g. if the screen is 10 rows by 108 cols, then the screen
// coordinate [3, 22] converts to 346
int ind(row, col) {
    return (screen.cols * row) + col;
}

// Sets the pixel in [row, col] with 'value'.
void set_pixel(row, col, value) {
    screen.pixels[ind(row,col)] = value;
}

void init_screen() {
    // See https://stackoverflow.com/a/1022961
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    screen.rows = w.ws_row;
    screen.cols = w.ws_col;
    screen.total_pixel_count = screen.rows * screen.cols;

    screen.pixels = (char*)malloc(screen.total_pixel_count * sizeof(char));

    // Initialise the screen borders.
    for (int i = 0; i < screen.rows; i++) {
        for (int j = 0; j < screen.cols; j++) {

            bool is_border = 
                (i == 0 || i == screen.rows-1) ||
                (j == 0 || j == screen.cols - 1);

            set_pixel(i, j, is_border ? '#' : ' ');
        }
    }
}

void clear_stdout() {
    system("clear");
}

void print_screen() {
    clear_stdout();

    // Reset the value of any pixel that isn't a border.
    for (int i = 1; i < screen.rows-1; i++) {
        for (int j = 1; j < screen.cols-1; j++) {
            set_pixel(i, j, ' ');
        }
    }

    // Set snake positions.
    for (int i = 0; i < screen.total_pixel_count; i++) {
        // printf("%c", screen.pixels[i]);
        int row_index = snake.row_coords[i];
        int col_index = snake.col_coords[i];

        // Snake is not yet this long, so break out of loop.
        if (row_index == 0 && col_index == 0) {
            break;
        }

        set_pixel(row_index, col_index, 'S');
    }

    for (int i = 0; i < screen.total_pixel_count; i++) {
        printf("%c", screen.pixels[i]);
    }
    printf("\n");
}

void init_snake() {
    // Assume init_screen() has been called.

    snake.row_coords = (int*)malloc(screen.total_pixel_count * sizeof(int));
    snake.col_coords = (int*)malloc(screen.total_pixel_count * sizeof(int));

    // Set initial position in middle of screen.
    snake.row_coords[0] = screen.rows / 2;
    snake.col_coords[0] = screen.cols / 2;

    // Set the initial movement direction of the snake (left).
    snake.direction[0] = 0;
    snake.direction[1] = -1;
}

void move_snake() {
    for (int i = 0; i < screen.total_pixel_count; i++) {
        int row_coord = snake.row_coords[i];
        int col_coord = snake.col_coords[i];

        if (!row_coord && !col_coord) {
            return;
        }

        snake.row_coords[i] += snake.direction[0];
        snake.col_coords[i] += snake.direction[1];
    }
}

// Copied from https://stackoverflow.com/a/1157217
int msleep(long msec)
{
    struct timespec ts;
    int res;
    if (msec < 0) { errno = EINVAL; return -1; }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do { res = nanosleep(&ts, &ts); } while (res && errno == EINTR);
    return res;
}

void free_all() {
    free(screen.pixels);

    free(snake.row_coords);
    free(snake.col_coords);
}

int main (int argc, char **argv) {
    init_screen();
    init_snake();

    while (true) {
        print_screen();
        move_snake();
        msleep(1000);
    }

    free_all();
    return 0;
}
