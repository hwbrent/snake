#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

// The dimensions of the screen.
int rows, cols;

int main(int argc, char **argv) {
    initscr();
    getmaxyx(stdscr,rows,cols);

    refresh();
    int d = getch();
    endwin();
    return 0;
}
