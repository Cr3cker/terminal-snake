# Snake Game

This project is a simple implementation of the classic Snake game using the ncurses library in C. The game is played in a terminal, and it allows the player to control a snake that moves around the screen, eats food, and grows longer. The objective is to avoid colliding with the walls or the snake's own body.

## Requirements

- GCC compiler
- ncurses library

## Installation

1. Ensure you have GCC and the ncurses library installed on your system. You can install them using your package manager. For example, on Debian-based systems:

    ```bash
    sudo apt-get install gcc libncurses5-dev libncursesw5-dev
    ```

2. Clone this repository or download the source code.

3. Compile the source code using the following command:

    ```bash
    gcc main.c -lncurses -o main
    ```

## Usage

1. Run the compiled program:

    ```bash
    ./main
    ```

2. Control the snake using the following keys:
    - `w` to move up
    - `s` to move down
    - `d` to move right
    - `a` to move left
    - `q` to quit the game

## Code Overview

### Snake Part Structure
A struct to define the snake's body parts.

### Grid Render Function
Renders the grid and snake on the screen.

### Update Grid Function
Updates the grid with the snake's new position.

### Create Walls Function
Creates the boundaries of the grid.

### Spawn Food Function
Randomly spawns food on the grid.

### Main Function
The `main` function initializes the game, sets up the ncurses environment, and starts the game loop. The game loop handles user input, updates the snake's position, checks for collisions, and updates the display.

## Game Mechanics

- The snake starts at a random position on the grid.
- The player controls the snake to move up, down, left, or right.
- The snake grows longer when it eats food.
- The game ends if the snake collides with the walls or its own body.

Enjoy the game!
