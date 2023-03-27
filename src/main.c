#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <time.h>

typedef struct pixel {
    int row;
    int col;
    char value;
} pixel;

struct screen {
    int rows; // the number of rows in the screen
    int cols; // the number of columns in the screen
    int total_pixels;
    pixel* pixels; // the char value that each pixel in the screen holds
    int* empty_pixels; // indices of pixels in 'pixels' whose 'value' is an empty char.
} screen;

struct snake {
    pixel* segments;
    pixel* head;
    bool should_add_seg;
} snake;

pixel food;

/* -------------------------------------------------- */

int ind(int row, int col) {
    return (screen.cols * row) + col;
}

pixel get_pixel(int i) {
    return screen.pixels[i];
}

void set_pixel(int i, pixel p) {
    screen.pixels[i] = p;
}

pixel create_pixel(int row, int col, char value) {
    pixel p;
    p.row = row;
    p.col = col;
    p.value = value;
    return p;
}

void init_screen() {
    initscr();
    getmaxyx(stdscr, screen.rows , screen.cols);
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    screen.total_pixels = screen.rows * screen.cols;

    screen.pixels = (pixel*)malloc(screen.total_pixels * sizeof(pixel));
    for (int i = 0; i < screen.rows; i++) {
        bool is_top_bottom_border =
            (i == 0) || (i == screen.rows-1);
        for (int j = 0; j < screen.cols; j++) {
            bool is_left_right_border = 
                (j == 0) || (j == screen.cols-1);
            bool is_border = (is_top_bottom_border || is_left_right_border);

            set_pixel(
                ind(i,j),
                create_pixel(i,j, (is_border) ? '#' : ' ')
            );
        }
    }
}

void terminate_screen() {
    endwin();
    free(screen.pixels);
    free(screen.empty_pixels);
}

void print_screen() {
    for (int i = 0; i < screen.total_pixels; i++) {
        pixel p = get_pixel(i);
        mvaddch(p.row, p.col, p.value);
    }
    refresh();
}

/* -------------------------------------------------- */

void init_snake() {
    snake.segments = (pixel*)malloc(sizeof(pixel));
    snake.head = &(snake.segments[0]);
    *snake.head = create_pixel(screen.rows / 2, screen.cols / 2, 'S');
    set_pixel(
        ind((*snake.head).row, (*snake.head).col),
        *snake.head
    );
} 
void terminate_snake() {
    free(snake.segments);
}

/* -------------------------------------------------- */

void init_food() {
    srand(time(NULL));
    food.value = 'F';
}
void terminate_food() {}

void place_food() {
    int empty_count = 0;
    screen.empty_pixels = malloc(0);

    for (int i = 1; i < screen.rows-1; i++) {
        for (int j = 1; j < screen.cols-1; j++) {
            int index = ind(i,j);
            if (get_pixel(index).value != ' ') {
                continue;
            }

            empty_count++;

            // Increment the size of the array.
            screen.empty_pixels = (int*)realloc(screen.empty_pixels, empty_count * sizeof(int));

            screen.empty_pixels[empty_count-1] = index;
        }
    }

    // Pick a random value from screen.empty_pixels
    int random_index = rand() % (empty_count-1) + 0;
    pixel prev_pixel = get_pixel(random_index);

    food.row = prev_pixel.row;
    food.col = prev_pixel.col;

    set_pixel(random_index, food);
}

/* -------------------------------------------------- */

void init_game() {
    init_screen();
    init_snake();
    init_food();
    place_food();
}
void terminate_game() {
    terminate_screen();
    terminate_snake();
    terminate_food();
}

/* -------------------------------------------------- */

void run_game() {
    print_screen();
    getch();
}

/* -------------------------------------------------- */

int main(int argc, char **argv) {
    init_game();
    run_game();
    terminate_game();
    return 0;
}
