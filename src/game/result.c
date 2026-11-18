#include <game.h>

static GameResult result = {.goal = GAME_GOAL_POINTS};

void game_result_set(uint16_t points, bool won) {
    result.points = points;
    result.won = won;
}

const GameResult *game_result_get(void) { return &result; }
