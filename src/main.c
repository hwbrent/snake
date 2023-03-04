#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

// The dimensions of the terminal in pixels.
int rows;
int cols;
void get_terminal_dimensions() {
    // See https://stackoverflow.com/a/1022961
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    rows = w.ws_row;
    cols = w.ws_col;
}

void clear_stdout() {
    system("clear");
}

// The coordinates of each segment of the snake.
int snake_rows[81];
int snake_cols[81];

void set_initial_snake_position() {
    int row_num = rows/2;
    int col_num = cols/2;

    snake_rows[0] = row_num;
    snake_cols[0] = col_num;

    for (int i = 1; i < 81; i++) {
        snake_rows[i] = -1;
        snake_cols[i] = -1;
    }
}

void print_snake() {
    for (int i = 0; i < 81; i++) {
        printf("[%d, %d], ", snake_rows[i], snake_cols[i]);
    }
    printf("\n");
}

int main (int argc, char **argv)
{
    get_terminal_dimensions();
    set_initial_snake_position();
    print_snake();
    return 0;
}