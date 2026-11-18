#include <input.h>
#include <termios.h>
#include <unistd.h>

static struct termios original;

void input_init(void) {
    tcgetattr(STDIN_FILENO, &original);

    struct termios raw = original;
    raw.c_lflag &= ~(ICANON | ECHO);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void input_shutdown(void) { tcsetattr(STDIN_FILENO, TCSANOW, &original); }

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
