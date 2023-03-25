#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

int c;

struct screen {
    int rows;
    int cols;
    int total_pixel_count;
    char* pixels;
} screen;

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
    /* ncurses initialisation details. */
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    /* My custom initialisation details. */
    getmaxyx(stdscr, screen.rows, screen.cols);
    screen.total_pixel_count = screen.rows * screen.cols;
    screen.pixels = (char*)malloc(screen.total_pixel_count * sizeof(char));

    // Initialise the screen borders.
    for (int i = 0; i < screen.rows; i++) {

        // If it's the first or last row, every pixel will be a hashtag.
        if (i == 0 || i == screen.rows - 1) {
            for (int j = 0; j < screen.cols; j++) {
                set_pixel(i, j, '#');
            }

        // Else, only the first and last pixels will be hashtags. The rest
        // will be empty space.
        } else {
            set_pixel(i, 0, '#');
            // for (int j = 1; j < screen.cols-2; j++) {
            //     set_pixel(i, screen.cols-1, ' ');
            // }
            set_pixel(i, screen.cols-1, '#');
        }
    }
}

void print_screen() {
    // Reset the value of any pixel that isn't a border.
    for (int i = 1; i < screen.rows-1; i++) {
        for (int j = 1; j < screen.cols-1; j++) {
            set_pixel(i, j, ' ');
        }
    }

    // Now actually print screen.
    for (int i = 0; i < screen.rows; i++) {
        for (int j = 0; j < screen.cols; j++) {
            char pixel = screen.pixels[ind(i,j)];
            mvaddch(i,j,pixel);
        }
    }
    refresh();
}

void terminate_screen() {
    endwin();
    free(screen.pixels);
}

int handle_key_press() {
    c = getch();

    switch (c) {
        case 'q':
            return 1;

        case 'w':
        case KEY_UP:
            printw("Pressed up\n");
            break;

        case 'a':
        case KEY_LEFT:
            printw("Pressed left\n");
            break;

        case 's':
        case KEY_DOWN:
            printw("Pressed down\n");
            break;

        case 'd':
        case KEY_RIGHT:
            printw("Pressed right\n");
            break;

        default:
            break;
    }

    return 0;
}

int main(int argc, char **argv) {
    init_screen();

    print_screen();
    getch();

    terminate_screen();
    return 0;
}
