#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

struct screen {
    int rows;
    int cols;
    int total_pixel_count;
    char* pixels;
};
struct screen screen;

void init_screen() {
    // See https://stackoverflow.com/a/1022961
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    screen.rows = w.ws_row;
    screen.cols = w.ws_col;
    screen.total_pixel_count = screen.rows * screen.cols;

    screen.pixels = (char*)malloc(screen.total_pixel_count * sizeof(char));
}

void clear_stdout() {
    system("clear");
}

struct snake {
    int* row_coords;
    int* col_coords;
};

struct snake snake;

void init_snake() {
    // Assume init_screen() has been called.

    snake.row_coords = (int*)malloc(screen.total_pixel_count * sizeof(int));
    snake.col_coords = (int*)malloc(screen.total_pixel_count * sizeof(int));

    // Set initial position in middle of screen.
    snake.row_coords[0] = screen.rows / 2;
    snake.col_coords[0] = screen.cols / 2;
}

void free_all() {
    free(screen.pixels);

    free(snake.row_coords);
    free(snake.col_coords);
}

int main (int argc, char **argv)
{
    printf("\n\n");

    init_screen();
    printf("screen: %d rows x %d cols\n", screen.rows, screen.cols);

    init_snake();
    printf("snake starting position: row %d, col %d\n", snake.row_coords[0], snake.col_coords[0]);

    free_all();
    printf("\n\n");
    return 0;
}