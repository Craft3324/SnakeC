#include <game.h>
#include <input.h>
#include <platform.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static bool tick_due(uint32_t *last_tick) {
    uint32_t now = platform_ticks_ms();

    if (now - *last_tick < SNAKE_TICK_MS) {
        return false;
    }

    *last_tick = now;
    return true;
}

static void game_handle_input(GameWorld *world, Input input) {
    switch (input) {
        case INPUT_UP:
            snake_turn(&world->snake, 0, -1);
            break;
        case INPUT_DOWN:
            snake_turn(&world->snake, 0, 1);
            break;
        case INPUT_LEFT:
            snake_turn(&world->snake, -1, 0);
            break;
        case INPUT_RIGHT:
            snake_turn(&world->snake, 1, 0);
            break;
        default:
            break;
    }
}

static void game_draw(const GameWorld *world, uint16_t points,
                      time_t started_at) {
    uint32_t elapsed = (uint32_t)(time(NULL) - started_at);

    printf("\033[H");
    game_render_stats(points, GAME_GOAL_POINTS, elapsed);
    game_render(world);
    fflush(stdout);
}

static Screen game_run(void) {
    uint16_t points = 0;
    bool won = false;
    time_t started_at = time(NULL);

    GameWorld world;

    game_init(&world);
    fruit_spawn(&world);

    uint32_t last_tick = platform_ticks_ms();

    game_draw(&world, points, started_at);

    while (true) {
        Input input = input_poll();

        if (input == INPUT_QUIT) {
            break;
        }

        game_handle_input(&world, input);

        if (!tick_due(&last_tick)) {
            continue;
        }

        SnakeResult result = snake_step(&world);

        if (result == SNAKE_DEAD) {
            break;
        }

        if (result == SNAKE_ATE) {
            points++;

            if (points >= GAME_GOAL_POINTS) {
                won = true;
                break;
            }

            fruit_spawn(&world);
        }

        game_draw(&world, points, started_at);
    }

    game_result_set(points, won);

    return SCREEN_GAME_OVER;
}

ScreenHandler game = {.run = game_run};
