#include <input.h>
#include <platform.h>
#include <screen.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
    input_init();
    platform_cursor_hide();

    Screen screen = SCREEN_MENU;
    while (screen != SCREEN_DONE) {
        switch (screen) {
            case SCREEN_MENU:
                screen = menu.run();
                break;
            case SCREEN_CREDITS:
                screen = credits.run();
                break;
            case SCREEN_PLAYING:
                screen = game.run();
                break;
            case SCREEN_GAME_OVER:
                screen = game_over.run();
                break;
            case SCREEN_DONE:
                break;
        }
    }

    printf("\033[2J\033[H");
    platform_cursor_show();
    input_shutdown();
}
