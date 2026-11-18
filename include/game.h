#pragma once

#include <screen.h>
#include <stdbool.h>
#include <stdint.h>

#define GAME_WIDTH 16
#define GAME_HEIGHT 16

#define SNAKE_START_X (GAME_WIDTH / 2)
#define SNAKE_START_Y (GAME_HEIGHT / 2)

#define SNAKE_START_LENGTH 3
#define SNAKE_MAX_LENGTH (GAME_WIDTH * GAME_HEIGHT)

#define GAME_GOAL_POINTS                                                       \
    ((GAME_WIDTH - 2) * (GAME_HEIGHT - 2) - SNAKE_START_LENGTH)

#define SNAKE_TICK_MS 200
#define SNAKE_HEAD "@"
#define SNAKE_BODY "o"

typedef struct {
    uint8_t x;
    uint8_t y;
} SnakeSegment;

typedef struct {
    SnakeSegment body[SNAKE_MAX_LENGTH];
    uint16_t length;
    int8_t dx;
    int8_t dy;
} Snake;

typedef struct {
    char tiles[GAME_HEIGHT][GAME_WIDTH];
    Snake snake;
} GameWorld;

typedef enum {
    SNAKE_DEAD,
    SNAKE_MOVED,
    SNAKE_ATE,
} SnakeResult;

typedef struct {
    uint16_t points;
    uint16_t goal;
    bool won;
} GameResult;

void game_init(GameWorld *world);
void game_render(const GameWorld *world);
void game_render_stats(uint16_t points, uint16_t goal, uint32_t elapsed);
bool fruit_spawn(GameWorld *world);

void game_result_set(uint16_t points, bool won);
const GameResult *game_result_get(void);

void snake_init(GameWorld *world);
void snake_turn(Snake *snake, int8_t dx, int8_t dy);
SnakeResult snake_step(GameWorld *world);
