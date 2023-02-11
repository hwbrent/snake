import os
import random
import keyboard
import time


def print_to_stdout(stdout, snake, food):
    for x, y in snake:
        stdout[y][x] = "S"

    foodx, foody = food
    stdout[foody][foodx] = "F"

    for line in stdout:
        print("".join(line))


def process_key_press(direction):
    event = keyboard.read_event()

    if event.event_type != keyboard.KEY_DOWN:
        return

    up = {"up", "w"}
    down = {"down", "s"}
    left = {"left", "a"}
    right = {"right", "d"}

    key = event.name
    if key in up:
        direction[0], direction[1] = 0, -1
    elif key in down:
        direction[0], direction[1] = 0, 1
    elif key in left:
        direction[0], direction[1] = -1, 0
    elif key in right:
        direction[0], direction[1] = 1, 0


def main():
    columns, lines = os.get_terminal_size()
    snake = [[columns // 2, lines // 2]]
    direction = [1, 0]

    food = [random.randint(0, columns), random.randint(0, lines)]
    stdout = [[" " for y in range(columns)] for x in range(lines)]

    while True:
        print_to_stdout(stdout, snake, food)

        # Check to see if there was a key press, and if necessary, adjust
        # the direction of movement of the snake.
        process_key_press(direction)

        # Move the head of the snake.
        snake[0][0] += direction[0]
        snake[0][1] += direction[1]

        # Move the rest of the parts of the snake to be where the preceding
        # part was in the step before.

        # Wait a bit before iterating.
        time.sleep(0.0125)


if __name__ == "__main__":
    main()
