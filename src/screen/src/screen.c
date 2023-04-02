#include <stdlib.h>
#include <ncurses.h>

#include "../../game/src/game.h"
#include "screen.h"

void init_screen() {
    system("clear");

    initscr();

    // Assign the numbers of rows and columns to 'screen.rows' and 'screen.cols'.
    getmaxyx(stdscr, screen.rows , screen.cols);

    cbreak();

    // Enables reading of arrow keys, F1, F2, etc.
    keypad(stdscr, TRUE);

    // Stops characters being echoed when typed.
    noecho();

    // Makes the cursor invisible.
    curs_set(0);

    // Draw the outer borders of the screen.
    for (int i = 0; i < screen.rows; i++) {
        for (int j = 0; j < screen.cols; j++) {
            bool is_border =
                i == 0 ||
                j == 0 ||
                i == screen.rows-1 ||
                j == screen.cols-1;
            mvaddch(i, j, is_border ? BORDER : EMPTY);
        }
    }
}

void terminate_screen() {
    endwin();
}
