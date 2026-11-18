#pragma once

#define RESET "\033[0m"
#define BG "\033[47m"
#define BLACK "\033[30m"

typedef enum {
    SCREEN_MENU,
    SCREEN_CREDITS,
    SCREEN_PLAYING,
    SCREEN_GAME_OVER,
    SCREEN_DONE,
} Screen;

typedef struct {
    Screen (*run)(void);
} ScreenHandler;

extern ScreenHandler menu;
extern ScreenHandler credits;
extern ScreenHandler game;
extern ScreenHandler game_over;
