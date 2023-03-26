#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

// For 'msleep'
#include <time.h>
#include <errno.h>

// For threading.
#include <pthread.h>

int c;

struct screen {
    int rows;
    int cols;
    int total_pixel_count;
    char* pixels;
} screen;

struct snake {
    int* row_coords;
    int* col_coords;
    int direction[2];
} snake;

struct food {
    int row;
    int col;
} food;

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
    // halfdelay(3); // 0.3s timeout
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

    // Set snake positions.
    for (int i = 0; i < screen.total_pixel_count; i++) {
        int row_index = snake.row_coords[i];
        int col_index = snake.col_coords[i];

        // Snake is not yet this long, so break out of loop.
        if (row_index == 0 && col_index == 0) {
            break;
        }

        set_pixel(row_index, col_index, 'S');
    }

    // Set food position.
    set_pixel(food.row, food.col, 'F');

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

void init_snake() {
    snake.row_coords = (int*)malloc(screen.total_pixel_count * sizeof(int));
    snake.col_coords = (int*)malloc(screen.total_pixel_count * sizeof(int));

    // Set initial position in middle of screen.
    snake.row_coords[0] = screen.rows / 2;
    snake.col_coords[0] = screen.cols / 2;

    // Set the initial movement direction of the snake (left).
    snake.direction[0] = 0;
    snake.direction[1] = -1;
}

void move_snake() {
    // The overall premise is to make the current segment equal to
    // the second most recent position of the preceding segment.

    int prev[2] = { snake.row_coords[0], snake.col_coords[0] };

    for (int i = 0; i < screen.total_pixel_count; i++) {

        if (i == 0) {
            // This is the head of the snake. Move it according to
            // 'snake.direction'.
            snake.row_coords[i] += snake.direction[0];
            snake.col_coords[i] += snake.direction[1];
            continue;
        }

        // Else, this is NOT the head of the snake.

        bool segment_is_undefined = 
            snake.row_coords[i] == 0 &&
            snake.col_coords[i] == 0;

        if (segment_is_undefined) {
            break;
        }

        // Save what this segment's coordinates USED to be.
        int temp[2] = { snake.row_coords[i], snake.col_coords[i] };

        // Set this segment's coords the same as 'prev'.
        snake.row_coords[i] = prev[0];
        snake.col_coords[i] = prev[1];

        // Update 'prev'.
        prev[0] = temp[0];
        prev[1] = temp[1];
    }
}

void terminate_snake() {
    free(snake.row_coords);
    free(snake.col_coords);
}

void init_food() {
    // Randomly generate a set of coordinates.
    // The coordinate cannot lie on a border or in the centre of the screen.

    srand(time(NULL));
}

// See https://stackoverflow.com/a/1190911
void place_food() {
    // // Row coordinate has to be in this range (both ends inclusive)
    // int row_coord[] = { 1, screen.rows - 2};

    // // Column coordinate has to be in this range (both ends inclusive)
    // int col_coord[] = { 1, screen.cols - 2};

    int row_coord;
    int col_coord;
    bool found_valid_coords;

    while (true) {
        row_coord = rand() % (screen.rows-2) + 1;
        col_coord = rand() % (screen.cols-2) + 1;

        // Check if the snake is already occupying the randomly-generated
        // space.
        found_valid_coords = true;
        for (int i = 0; i < screen.total_pixel_count; i++) {
            if (snake.row_coords[i] == row_coord && snake.col_coords[i] == col_coord) {
                found_valid_coords = false;
                break;
            }
        }

        if (found_valid_coords) {
            break;
        }
    }

    food.row = row_coord;
    food.col = col_coord;
}

// Copied from https://stackoverflow.com/a/1157217
int msleep(long msec)
{
    struct timespec ts;
    int res;
    if (msec < 0) { errno = EINVAL; return -1; }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do { res = nanosleep(&ts, &ts); } while (res && errno == EINTR);
    return res;
}

void *getch_thread_fn(void *vargp) {
    while (1) {
        c = getch();
        switch (c) {
            case 'w':
            case KEY_UP:
                snake.direction[0] = -1;
                snake.direction[1] = 0;
                break;

            case 'a':
            case KEY_LEFT:
                snake.direction[0] = 0;
                snake.direction[1] = -1;
                break;

            case 's':
            case KEY_DOWN:
                snake.direction[0] = 1;
                snake.direction[1] = 0;
                break;

            case 'd':
            case KEY_RIGHT:
                snake.direction[0] = 0;
                snake.direction[1] = 1;
                break;

            default:
                break;
        }
    }
}

void init_game() {
    init_screen();
    init_snake();
    init_food();
}

void terminate_game() {
    terminate_screen();
    terminate_snake();
}

void run_game() {
    // init_game();

    pthread_t ptid;
    pthread_create(&ptid, NULL, &getch_thread_fn, NULL);

    for (int i = 0; i < 20; i++) {
        if (c == 'q') {
            break;
        }

        print_screen();
        move_snake();
        msleep(125);
    }
    pthread_cancel(ptid);

    getch();

    // terminate_game();
}

int main(int argc, char **argv) {
    init_game();

    place_food();

    print_screen();
    getch();

    terminate_game();
    return 0;
}
