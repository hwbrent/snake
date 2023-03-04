#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

// The dimensions of the terminal in pixels.
int rows;
int cols;
void get_terminal_dimensions() {
    // See https://stackoverflow.com/a/1022961
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    rows = w.ws_row;
    cols = w.ws_col;
}

void clear_stdout() {
    system("clear");
}

int main (int argc, char **argv)
{
    get_terminal_dimensions();
    printf ("rows %d\n", rows);
    printf ("cols %d\n", cols);
    return 0;  // make sure your main returns int
}