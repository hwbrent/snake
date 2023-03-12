#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "screen.h"

struct terminal_window {
    int rows;
    int cols;
};

struct terminal_window terminal;

void get_terminal_dimensions() {
    // See https://stackoverflow.com/a/1022961
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    terminal.rows = w.ws_row;
    terminal.cols = w.ws_col;

    printf("\nscreen -> %d x %d\n\n", terminal.rows, terminal.cols);
}
