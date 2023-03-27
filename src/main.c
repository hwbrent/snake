#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

struct coord {
    int row;
    int col;
    char value;
};

struct screen {
    int rows; // the number of rows in the screen
    int cols; // the number of columns in the screen
    char* pixels; // the char value that each pixel in the screen holds
    int* empty_pixel_rows; // the row coordinates of the empty (' ') pixels in the screen
    int* empty_pixel_cols; // the column coordinates of the empty (' ') pixels in the screen
} screen;

struct snake {
    int* row_coords;
    int* col_coords;
    int head[2];
    bool should_add_seg;
} snake;

struct food {
    int row;
    int col;
} food;

int main(int argc, char **argv) {
    return 0;
}
