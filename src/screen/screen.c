#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "screen.h"

void get_terminal_dimensions() {
    // See https://stackoverflow.com/a/1022961
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    printf("\nscreen -> %d x %d\n\n", w.ws_row, w.ws_col);
}
