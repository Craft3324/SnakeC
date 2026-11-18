#include <game.h>
#include <stdint.h>
#include <stdlib.h>

bool fruit_spawn(GameWorld *world) {
    uint16_t empty[GAME_WIDTH * GAME_HEIGHT];
    uint16_t count = 0;

    for (uint16_t y = 1; y < GAME_HEIGHT - 1; y++) {
        for (uint16_t x = 1; x < GAME_WIDTH - 1; x++) {
            if (world->tiles[y][x] == ' ') {
                empty[count++] = y * GAME_WIDTH + x;
            }
        }
    }

    if (count == 0) {
        return false;
    };

    uint16_t position = empty[rand() % count];
    world->tiles[position / GAME_WIDTH][position % GAME_WIDTH] = '*';

    return true;
}
