#include <game.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define GAME_RENDER_WIDTH ((GAME_WIDTH * 2) - 1)

void game_render_stats(uint16_t points, uint16_t goal, uint32_t elapsed) {
    char left[48];
    char right[32];

    snprintf(left, sizeof(left), "Points: %u / %u", points, goal);
    snprintf(right, sizeof(right), "Time: %us", elapsed);

    int gap = GAME_RENDER_WIDTH - (int)strlen(left) - (int)strlen(right);

    if (gap < 1) {
        gap = 1;
    }

    printf("%s%*s%s\n", left, gap, "", right);
}
