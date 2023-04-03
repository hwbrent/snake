#include <stdlib.h>
#include <ncurses.h>

#include "../../game/src/game.h"
#include "screen.h"

void init_screen() {
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

void terminate_screen() {}
