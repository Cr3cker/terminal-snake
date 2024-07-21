#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 15
#define SNAKE 2
#define FOOD 3
#define SNAKE_LENGTH 100
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
void spawn_food(unsigned int grid[HEIGHT][WIDTH]);
int check_collision(int new_x, int new_y, snake_part* body, int length);
void check_food_eaten(unsigned int grid[HEIGHT][WIDTH], int new_x, int new_y, int* length, int* score, int* eaten);

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    srand(time(NULL));
    timeout(100); 
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    snake_part body[SNAKE_LENGTH];
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
    spawn_food(grid);

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

        if (check_collision(new_x, new_y, body, length)) {
            endwin(); 
            printf("Game Over! Your highest score is %d!\n", score);
            return 0;
        }

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

        for (int i = length; i > 0; i--) {
            body[i] = body[i - 1];
        }
        body[0].first = new_x;
        body[0].second = new_y;

        check_food_eaten(grid, new_x, new_y, &length, &score, &eaten);
        update_grid(grid, body, length);
        clear();
        grid_render(grid, body[0].first, body[0].second, body, length);
        refresh();

        usleep(100000 - (1000 * score));
    }
    endwin();
    return 0;
}

void check_food_eaten(unsigned int grid[HEIGHT][WIDTH], int new_x, int new_y, int* length, int* score, int* eaten) {
    if (grid[new_x][new_y] == 3) {
        (*length)++;
        (*score)++;
        spawn_food(grid);  
    } 
}

int check_collision(int new_x, int new_y, snake_part* body, int length) {
    if (new_x < 1 || new_x >= HEIGHT - 1 || new_y < 1 || new_y >= WIDTH - 1) return 1; 
            for (int i = 1; i < length; i++) 
                if (new_x == body[i].first && new_y == body[i].second) return 1;
    return 0;
}

void create_walls(unsigned int grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) grid[i][j] ^= 1;
        }
    }
}

void grid_render(unsigned int grid[HEIGHT][WIDTH], int snake_x, int snake_y, snake_part* body, int length) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) addch('#' | A_BOLD);
            else if (!grid[i][j]) addch(' ');
            else if (grid[i][j] == SNAKE) {
                attron(COLOR_PAIR(1));
                addch('*' | A_BOLD);
                attroff(COLOR_PAIR(1));
            }
            else if (grid[i][j] == FOOD) addch('.' | A_BOLD);
        }
        addch('\n');
    }
}

void update_grid(unsigned int grid[HEIGHT][WIDTH], snake_part* body, int length) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] == SNAKE) grid[i][j] = 0;
        }
    }

    for (int i = 0; i < length; i++) {
        grid[body[i].first][body[i].second] = SNAKE;
    }
}

void spawn_food(unsigned int grid[HEIGHT][WIDTH]) {
    int food_x, food_y;
    do {
        food_x = rand() % (HEIGHT - 2) + 1;
        food_y = rand() % (WIDTH - 2) + 1;
    } while (grid[food_x][food_y] != 0);
    grid[food_x][food_y] = FOOD;
}