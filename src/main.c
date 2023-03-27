#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

typedef struct pixel {
    int row;
    int col;
    char value;
} pixel;

struct screen {
    int rows; // the number of rows in the screen
    int cols; // the number of columns in the screen
    pixel* pixels; // the char value that each pixel in the screen holds
    int* empty_pixels; // indices of pixels in 'pixels' whose 'value' is an empty char.
} screen;

struct snake {
    pixel* segments;
    pixel head;
    bool should_add_seg;
} snake;

struct food {
    pixel coord;
} food;

int main(int argc, char **argv) {
    return 0;
}
