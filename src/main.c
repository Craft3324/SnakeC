#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 20
#define MAX_LENGTH (WIDTH * HEIGHT)

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position body[MAX_LENGTH];
    int length;
    char direction;
} Snake;

typedef struct {
    int x;
    int y;
} Food;

int kbhitInGame(void) {
    return _kbhit();
}

char getKey(void) {
    return getch();
}

void clearScreen(void) {
    system("cls");
}

void hideCursor(void) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursor);
}

void moveCursor(int x, int y) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(console, position);
}

int positionIsOnSnake(Snake *snake, int x, int y) {
    for (int i = 0; i < snake->length; i++) {
        if (snake->body[i].x == x && snake->body[i].y == y) {
            return 1;
        }
    }
    return 0;
}

void placeFood(Snake *snake, Food *food) {
    do {
        food->x = 1 + rand() % WIDTH;
        food->y = 1 + rand() % HEIGHT;
    } while (positionIsOnSnake(snake, food->x, food->y));
}

void changeDirection(Snake *snake, char key) {
    if (key == 'w' && snake->direction != 's') {
        snake->direction = 'w';
    }
    if (key == 's' && snake->direction != 'w') {
        snake->direction = 's';
    }
    if (key == 'a' && snake->direction != 'd') {
        snake->direction = 'a';
    }
    if (key == 'd' && snake->direction != 'a') {
        snake->direction = 'd';
    }
}

int snakeHitsWall(Snake *snake) {
    int headX = snake->body[0].x;
    int headY = snake->body[0].y;

    return headX < 1 || headX > WIDTH ||
           headY < 1 || headY > HEIGHT;
}

int snakeHitsItself(Snake *snake) {
    int headX = snake->body[0].x;
    int headY = snake->body[0].y;

    for (int i = 1; i < snake->length; i++) {
        if (snake->body[i].x == headX &&
            snake->body[i].y == headY) {
            return 1;
        }
    }
    return 0;
}

void getNextPosition(Snake *snake, int *nextX, int *nextY) {
    *nextX = snake->body[0].x;
    *nextY = snake->body[0].y;

    if (snake->direction == 'w') {
        (*nextY)--;
    }
    if (snake->direction == 's') {
        (*nextY)++;
    }
    if (snake->direction == 'a') {
        (*nextX)--;
    }
    if (snake->direction == 'd') {
        (*nextX)++;
    }
}

void moveSnake(Snake *snake, int grow) {
    Position oldTail = snake->body[snake->length - 1];

    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    if (snake->direction == 'w') {
        snake->body[0].y--;
    }
    if (snake->direction == 's') {
        snake->body[0].y++;
    }
    if (snake->direction == 'a') {
        snake->body[0].x--;
    }
    if (snake->direction == 'd') {
        snake->body[0].x++;
    }

    if (grow == 1) {
        snake->body[snake->length] = oldTail;
        snake->length++;
    }
}

void drawBoard(void) {
    for (int y = 0; y <= HEIGHT + 1; y++) {
        for (int x = 0; x <= WIDTH + 1; x++) {
            moveCursor(x * 2, y + 2);
            if (y == 0 || y == HEIGHT + 1 ||
                x == 0 || x == WIDTH + 1) {
                printf("##");
            } else {
                printf(" ");
            }
        }
    }
}

void drawSnake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        moveCursor(snake->body[i].x * 2, snake->body[i].y + 2);
        printf("%c ", i == 0 ? 'O' : 'o');
        fflush(stdout);
    }
}

void clearSnake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        moveCursor(snake->body[i].x * 2, snake->body[i].y + 2);
        printf("  ");
        fflush(stdout);
    }
}

void drawFood(Food *food) {
    moveCursor(food->x * 2, food->y + 2);
    printf("* ");
    fflush(stdout);
}

void drawScore(int score) {
    moveCursor(0, 0);
    printf("################  SCORE: %d  ################", score);
    fflush(stdout);
}

int main(void) {
    srand((unsigned int)time(NULL));
    clearScreen();
    hideCursor();

    Snake snake;
    snake.length = 3;
    snake.direction = 'd';

    snake.body[0].x = 6;
    snake.body[0].y = 10;
    snake.body[1].x = 5;
    snake.body[1].y = 10;
    snake.body[2].x = 4;
    snake.body[2].y = 10;

    Food food;
    placeFood(&snake, &food);

    int score = 0;

    clearScreen();
    drawScore(score);
    drawBoard();
    drawFood(&food);
    drawSnake(&snake);

    while (1) {
        if (kbhitInGame()) {
            char key = getKey();
            changeDirection(&snake, key);
        }

        int nextX;
        int nextY;
        getNextPosition(&snake, &nextX, &nextY);

        if (nextX < 1 || nextX > WIDTH ||
            nextY < 1 || nextY > HEIGHT) {
            printf("\nGame over\n");
            break;
        }

        int ateFood = nextX == food.x && nextY == food.y;
        clearSnake(&snake);
        moveSnake(&snake, ateFood);

        if (snakeHitsWall(&snake)) {
            drawSnake(&snake);
            printf("\nGame over\n");
            break;
        }

        if (snakeHitsItself(&snake)) {
            drawSnake(&snake);
            printf("\nGame over\n");
            break;
        }

        if (ateFood) {
            score++;
            placeFood(&snake, &food);
        }

        drawSnake(&snake);
        drawFood(&food);
        drawScore(score);
        Sleep(250);
    }

    printf("Final score: %d\n", score);
    return 0;
}
