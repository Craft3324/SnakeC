#define _POSIX_C_SOURCE 200809L

#include <input.h>
#include <platform.h>
#include <stdio.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

static struct termios original;

uint32_t platform_ticks_ms(void) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (uint32_t)(now.tv_sec * 1000 + now.tv_nsec / 1000000);
}

void input_init(void) {
    tcgetattr(STDIN_FILENO, &original);
    struct termios raw = original;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void input_shutdown(void) { tcsetattr(STDIN_FILENO, TCSANOW, &original); }

void platform_cursor_hide(void) {
    fputs("\033[?25l", stdout);
    fflush(stdout);
}

void platform_cursor_show(void) {
    fputs("\033[?25h", stdout);
    fflush(stdout);
}

Input input_poll(void) {
    char key;
    if (read(STDIN_FILENO, &key, 1) != 1) {
        return INPUT_NONE;
    }
    if (key == '\033') {
        char sequence;
        if (read(STDIN_FILENO, &sequence, 1) != 1 || sequence != '[') {
            return INPUT_NONE;
        }
        if (read(STDIN_FILENO, &key, 1) != 1) {
            return INPUT_NONE;
        }
        switch (key) {
            case 'A':
                return INPUT_UP;
            case 'B':
                return INPUT_DOWN;
            case 'C':
                return INPUT_RIGHT;
            case 'D':
                return INPUT_LEFT;
        }

        return INPUT_NONE;
    }
    switch (key) {
        case 'w':
        case 'W':
            return INPUT_UP;
        case 's':
        case 'S':
            return INPUT_DOWN;
        case 'd':
        case 'D':
            return INPUT_RIGHT;
        case 'a':
        case 'A':
            return INPUT_LEFT;
        case '\n':
        case '\r':
        case ' ':
            return INPUT_SELECT;
        case 'q':
        case 'Q':
            return INPUT_QUIT;
    }
    return INPUT_NONE;
}
