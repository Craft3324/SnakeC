#include <input.h>
#include <screen.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static Screen menu_run(void) {
    static const char *const items[] = {"Start game", "Credits", "Quit"};
    const uint8_t item_count = sizeof(items) / sizeof(items[0]);
    uint8_t selected = 0;
    bool running = true;

    while (running) {
        printf("\033[2J\033[H");

        printf("\nThe Amazing Digital Snake Game\n\n");

        for (uint8_t i = 0; i < item_count; i++) {
            if (i == selected) {
                printf(BG BLACK " > %-20s " RESET "\n", items[i]);
            } else {
                printf("   %s\n", items[i]);
            }
        }

        fflush(stdout);
        switch (input_poll()) {
            case INPUT_UP:
                selected = (selected + item_count - 1) % item_count;
                break;
            case INPUT_DOWN:
                selected = (selected + 1) % item_count;
                break;
            case INPUT_SELECT:
                running = false;
                break;
            case INPUT_QUIT:
                return SCREEN_DONE;
            default:
                break;
        }
    }

    switch (selected) {
        case 0:
            return SCREEN_PLAYING;
        case 1:
            return SCREEN_CREDITS;
        case 2:
            return SCREEN_DONE;
    }

    return SCREEN_DONE;
}

ScreenHandler menu = {.run = menu_run};
