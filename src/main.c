#include <ncurses.h>

#include "game/src/game.h"
#include "gui/src/gui.h"
#include "screen/src/screen.h"

int main(int argc, char **argv) {
    initscr();
    getmaxyx(stdscr, screen.rows , screen.cols);
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    // init_game();
    // run_game();
    // terminate_game();

    printw("hi\n");
    refresh();
    getch();

    endwin();

    return 0;
}
