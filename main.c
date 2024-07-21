#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 15
#define SNAKE 10
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3


typedef struct {
    int first;
    int second;
} snake_part;

void grid_render(unsigned int grid[HEIGHT][WIDTH], int snake_x, int snake_y, snake_part* body, int length);
void update_grid(unsigned int grid[HEIGHT][WIDTH], snake_part* body, int length);
void create_walls(unsigned int grid[HEIGHT][WIDTH]);
void start_game(unsigned int grid[HEIGHT][WIDTH]);
void spawn_food(unsigned int grid[HEIGHT][WIDTH], int eaten);

int main() {
    initscr();
    cbreak();
    noecho();
    srand(time(NULL));
    timeout(100); 
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    snake_part body[SNAKE];
    int direction = RIGHT;
    unsigned int grid[HEIGHT][WIDTH] = { 0 };
    int snake_x = rand() % (HEIGHT - 2) + 1;
    int snake_y = rand() % (WIDTH - 2) + 1;
    body[0].first = snake_x;
    body[0].second = snake_y;
    int length = 1;
    int eaten = 1;
    int score = 0;

    create_walls(grid);
    spawn_food(grid, eaten);

    for (;;) {
        int ch = getch();
        switch (ch) {
            case 'q': break;
            case 'w': 
                direction = UP;         
                break;
            case 's':
                direction = DOWN;
                break;
            case 'd':
                direction = RIGHT;
                break;
            case 'a':
                direction = LEFT;
                break;
        }

        int new_x = body[0].first;
        int new_y = body[0].second;

        switch (direction) {
            case UP:
                new_x--;
                break;
            case DOWN:
                new_x++;
                break;
            case RIGHT:
                new_y++;
                break;
            case LEFT:
                new_y--;
                break;
        }

        if (new_x < 1 || new_x >= HEIGHT - 1 || new_y < 1 || new_y >= WIDTH - 1) break; 
            for (int i = 1; i < length; i++) { 
                if (new_x == body[i].first && new_y == body[i].second) {
                    endwin(); 
                    printf("Game Over! Collision with body.\n");
                    return 0;
                }
            }

        for (int i = length; i > 0; i--) {
            body[i] = body[i - 1];
        }
        body[0].first = new_x;
        body[0].second = new_y;

        if (grid[new_x][new_y] == 3) {
            length++;
            score++;
            eaten = 1;
            spawn_food(grid, eaten);  
        } else {
            eaten = 0;
        }

        update_grid(grid, body, length);
        clear();
        grid_render(grid, body[0].first, body[0].second, body, length);
        refresh();

        usleep(100000 - (1000 * score));
    }
    endwin();
    return 0;
}

void create_walls(unsigned int grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) grid[i][j] = (~grid[i][j]) & 1;
        }
    }
}

void grid_render(unsigned int grid[HEIGHT][WIDTH], int snake_x, int snake_y, snake_part* body, int length) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) addch('#' | A_BOLD);
            else if (!grid[i][j]) addch(' ');
            else if (grid[i][j] == 2) {
                attron(COLOR_PAIR(1));
                addch('*' | A_BOLD);
                attroff(COLOR_PAIR(1));
            }
            else if (grid[i][j] == 3) addch('.' | A_BOLD);
        }
        addch('\n');
    }
}

void update_grid(unsigned int grid[HEIGHT][WIDTH], snake_part* body, int length) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] == 2) grid[i][j] = 0;
        }
    }

    for (int i = 0; i < length; i++) {
        grid[body[i].first][body[i].second] = 2;
    }
}

void spawn_food(unsigned int grid[HEIGHT][WIDTH], int eaten) {
    int food_x, food_y;
    do {
        food_x = rand() % (HEIGHT - 2) + 1;
        food_y = rand() % (WIDTH - 2) + 1;
    } while (grid[food_x][food_y] != 0);
    if (eaten) grid[food_x][food_y] = 3;
}