#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <locale.h>

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
    snake_part body[SNAKE];
    initscr();
    setlocale(LC_ALL, "");
    cbreak();
    noecho();
    timeout(100); 
    int direction = RIGHT;
    unsigned int grid[HEIGHT][WIDTH] = { 0 };
    srand(time(NULL));
    int snake_x = rand() % (HEIGHT - 1);
    int snake_y = rand() % (WIDTH - 1);
    body[0].first = snake_x;
    body[0].second = snake_y;
    int length = 1;
    int eaten = 1;
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

        int old_x = body[0].first;
        int old_y = body[0].second;

        switch (direction) {
            case UP:
                body[0].first--;
                break;
            case DOWN:
                body[0].first++;
                break;
            case RIGHT:
                body[0].second++;
                break;
            case LEFT:
                body[0].second--;
                break;
        }
        
        if (body[0].first < 1) body[0].first = 1;
        else if (body[0].first >= HEIGHT - 1) body[0].first = HEIGHT - 2;
        else if (body[0].second < 1) body[0].second = 1;
        else if (body[0].second >= WIDTH - 1) body[0].second = WIDTH - 2;

        if (grid[body[0].first][body[0].second] == 3) {
            length++;
            if (length > SNAKE) {
                length = SNAKE;
            } else {
                for (int i = length - 1; i > 0; i--) {
                    body[i] = body[i - 1];
                }
            }
            spawn_food(grid, eaten);  
        } else {
            for (int i = length - 1; i > 0; i--) {
                body[i] = body[i - 1];
            }
        }

        update_grid(grid, body, length);
        clear();
        grid_render(grid, body[0].first, body[0].second, body, length);
        refresh();

        usleep(1000);
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
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) addch('X' | A_BOLD);
            else if (!grid[i][j]) addch(' ');
            else if (grid[i][j] == 2) addstr('*' | A_BOLD);
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
    int food_x = rand() % (HEIGHT - 1);
    int food_y = rand() % (WIDTH - 1);
    if (eaten) grid[food_x][food_y] = 3;
}