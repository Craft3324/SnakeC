#include <game.h>

void snake_init(GameWorld *world) {
    Snake *snake = &world->snake;

    snake->length = SNAKE_START_LENGTH;
    snake->dx = 1;
    snake->dy = 0;

    for (uint16_t i = 0; i < snake->length; i++) {
        snake->body[i].x = SNAKE_START_X - i;
        snake->body[i].y = SNAKE_START_Y;
        world->tiles[SNAKE_START_Y][SNAKE_START_X - i] = 'o';
    }

    world->tiles[SNAKE_START_Y][SNAKE_START_X] = '@';
}

void snake_turn(Snake *snake, int8_t dx, int8_t dy) {
    if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0)) {
        return;
    }

    if (snake->length > 1) {
        int8_t hx = (int8_t)(snake->body[0].x - snake->body[1].x);
        int8_t hy = (int8_t)(snake->body[0].y - snake->body[1].y);

        if (dx == -hx && dy == -hy) {
            return;
        }
    }

    snake->dx = dx;
    snake->dy = dy;
}

static bool snake_hits_body(const Snake *snake, uint16_t length, int x, int y) {
    for (uint16_t i = 0; i < length; i++) {
        if (snake->body[i].x == x && snake->body[i].y == y) {
            return true;
        }
    }

    return false;
}

SnakeResult snake_step(GameWorld *world) {
    Snake *snake = &world->snake;

    int x = snake->body[0].x + snake->dx;
    int y = snake->body[0].y + snake->dy;

    if (x <= 0 || x >= GAME_WIDTH - 1 || y <= 0 || y >= GAME_HEIGHT - 1) {
        return SNAKE_DEAD;
    }

    bool ate = world->tiles[y][x] == '*';

    uint16_t checked = snake->length - (ate ? 0 : 1);
    if (snake_hits_body(snake, checked, x, y)) {
        return SNAKE_DEAD;
    }

    bool grows = ate && snake->length < SNAKE_MAX_LENGTH;

    if (!grows) {
        SnakeSegment tail = snake->body[snake->length - 1];
        world->tiles[tail.y][tail.x] = ' ';
    }

    uint16_t new_length = snake->length + (grows ? 1 : 0);

    for (uint16_t i = new_length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    snake->length = new_length;
    snake->body[0].x = (uint8_t)x;
    snake->body[0].y = (uint8_t)y;

    world->tiles[snake->body[1].y][snake->body[1].x] = *SNAKE_BODY;
    world->tiles[y][x] = *SNAKE_HEAD;

    return ate ? SNAKE_ATE : SNAKE_MOVED;
}
