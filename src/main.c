#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

// The dimensions of the screen.
int rows, cols;

// Stores value of inputted chars.
int c;

void init_screen() {
    // Init the ncurses screen.
    initscr();

    // Get the terminal size.
    getmaxyx(stdscr,rows,cols);

    cbreak();

    // Pick up extra keyboard keys (arrows, F1-12, etc.)
    keypad(stdscr, TRUE);

    noecho();
}

void terminate_screen() {
    endwin();
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

    int should_quit = handle_key_press();
    if (!should_quit) {
        refresh();
        getch();
    }

    terminate_screen();
    return 0;
}
