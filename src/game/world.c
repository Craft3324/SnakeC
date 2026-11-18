#include <game.h>
#include <stdbool.h>
#include <stdio.h>

void game_init(GameWorld *world) {
    for (int y = 0; y < GAME_HEIGHT; y++) {
        for (int x = 0; x < GAME_WIDTH; x++) {
            bool border =
                x == 0 || x == GAME_WIDTH - 1 || y == 0 || y == GAME_HEIGHT - 1;
            world->tiles[y][x] = border ? '#' : ' ';
        }
    }

    snake_init(world);
}

void game_render(const GameWorld *world) {
    for (int y = 0; y < GAME_HEIGHT; y++) {
        for (int x = 0; x < GAME_WIDTH; x++) {
            printf("%c ", world->tiles[y][x]);
        }
        putchar('\n');
    }
}
