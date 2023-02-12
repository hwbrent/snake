import os
import random
import keyboard
import time
import copy
import sys


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


def move_food(food, snake, columns, lines):
    snake_xs = (seg[0] for seg in snake)
    snake_ys = (seg[1] for seg in snake)

    poss_xs = list(x for x in range(columns) if not x in snake_xs and x != food[0])
    poss_ys = list(y for y in range(lines) if not y in snake_ys and y != food[1])

    random_x = random.randint(min(poss_xs), max(poss_xs) - 1)
    random_y = random.randint(min(poss_ys), max(poss_ys) - 1)

    food[0], food[1] = random_x, random_y


def test():
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
        move_food(food, snake, columns, lines)
        time.sleep(0.0125)


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

        # Move the snake.
        preceding_seg = None
        for i, part in enumerate(snake):
            if i == 0:
                head = part
                preceding_seg = copy.deepcopy(head)
                head[0] += direction[0]
                head[1] += direction[1]
            else:
                current_seg = snake[i]
                current_copy = copy.deepcopy()

                current_seg[0], current_seg[0] = preceding_seg[0], preceding_seg[1]
                preceding_seg[0], preceding_seg[0] = current_copy[0], current_copy[1]

        # If the snake head is over the food, increment its length.
        if snake[0] == food:
            snake.append(preceding_seg)
            move_food(food, snake, columns, lines)

        # If the snake ate itself.
        if snake[0] in snake[1:]:
            print("GAME OVER")
            sys.exit()
        #

        # Move the rest of the parts of the snake to be where the preceding
        # part was in the step before.

        # Wait a bit before iterating.
        time.sleep(0.0125)


if __name__ == "__main__":
    # main()
    test()
