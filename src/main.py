import os
import random


def print_to_stdout(stdout, snake, food):
    for x, y in snake:
        stdout[y][x] = "S"

    foodx, foody = food
    stdout[foody][foodx] = "F"

    for line in stdout:
        print("".join(line))


def main():
    columns, lines = os.get_terminal_size()
    snake = [[columns // 2, lines // 2]]
    food = [random.randint(0, columns), random.randint(0, lines)]
    stdout = [[" " for y in range(columns)] for x in range(lines)]

    print_to_stdout(stdout, snake, food)


if __name__ == "__main__":
    main()
