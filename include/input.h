#pragma once

typedef enum {
    INPUT_NONE,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_SELECT,
    INPUT_QUIT
} Input;

void input_init(void);
void input_shutdown(void);
Input input_poll(void);
