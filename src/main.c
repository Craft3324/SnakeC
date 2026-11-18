#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define ROWS 10
#define COLS 30
#define MAX_SNAKE (ROWS * COLS)

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

typedef struct {
    int x;
    int y;
} Point;

Point snake[MAX_SNAKE];
Point food;
int snakeLength = 3;
int score = 0;
int direction = RIGHT;

int samePoint(Point a, Point b) {
    return a.x == b.x && a.y == b.y;
}

int onSnake(Point p) {
    for (int i = 0; i < snakeLength; i++) {
        if (samePoint(p, snake[i])) return 1;
    }
    return 0;
}

void placeFood(void) {
    if (snakeLength == MAX_SNAKE) return;

    do {
        food.x = rand() % COLS + 1;
        food.y = rand() % ROWS + 1;
    } while (onSnake(food));
}

void drawGame(void) {
    system("cls");

    for (int y = 0; y < ROWS + 2; y++) {
        for (int x = 0; x < COLS + 2; x++) {
            if (x == 0 || x == COLS + 1 || y == 0 || y == ROWS + 1) {
                putchar('#');
            } else {
                Point p = {x, y};
                char symbol = ' ';

                if (samePoint(p, snake[0])) symbol = 'O';
                else if (onSnake(p)) symbol = 'o';
                else if (samePoint(p, food)) symbol = '*';

                putchar(symbol);
            }
        }
        putchar('\n');
    }

    printf("Points: %d\n", score);
    printf("Styr med W, S, A, D. Avsluta med Q.\n");
}

int readInput(void) {
    if (!_kbhit()) return 1;

    int key = _getch();
    if (key == 'q' || key == 'Q') return 0;

    if ((key == 'w' || key == 'W') && direction != DOWN) direction = UP;
    else if ((key == 's' || key == 'S') && direction != UP) direction = DOWN;
    else if ((key == 'a' || key == 'A') && direction != RIGHT) direction = LEFT;
    else if ((key == 'd' || key == 'D') && direction != LEFT) direction = RIGHT;

    return 1;
}

int moveSnake(void) {
    Point next = snake[0];

    if (direction == UP) next.y--;
    else if (direction == DOWN) next.y++;
    else if (direction == LEFT) next.x--;
    else next.x++;

    // collision med vägg
    if (next.x < 1 || next.x > COLS || next.y < 1 || next.y > ROWS)
        return 0;

    int ateFood = samePoint(next, food);

    // kontrollerar collision med kroppen
    int partsToCheck = ateFood ? snakeLength : snakeLength - 1;
    for (int i = 0; i < partsToCheck; i++) {
        if (samePoint(next, snake[i])) return 0;
    }

    if (ateFood && snakeLength < MAX_SNAKE) snakeLength++;

    for (int i = snakeLength - 1; i > 0; i--)
        snake[i] = snake[i - 1];
    snake[0] = next;

    if (ateFood) {
        score++;
        placeFood();
    }

    return 1;
}

int main(void) {
    srand((unsigned int)time(NULL));

    snake[0].x = 5; snake[0].y = 5;
    snake[1].x = 4; snake[1].y = 5;
    snake[2].x = 3; snake[2].y = 5;
    placeFood();

    int running = 1;
    while (running) {
        if (!readInput()) break;

        running = moveSnake();
        drawGame();
    }

    printf("Spelet ar slut! Slutpoang: %d\n", score);
    return 0;
}
