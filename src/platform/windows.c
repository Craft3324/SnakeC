#include <conio.h>
#include <input.h>
#include <windows.h>

void input_init(void) {}

void input_shutdown(void) {}

Input input_poll(void) {
    if (!_kbhit()) {
        Sleep(100);
        return INPUT_NONE;
    }

    int c = _getch();

    if (c == 0 || c == 224) {
        c = _getch();

        switch (c) {
            case 72:
                return INPUT_UP;
            case 80:
                return INPUT_DOWN;
            case 75:
                return INPUT_LEFT;
            case 77:
                return INPUT_RIGHT;
        }

        return INPUT_NONE;
    }

    switch (c) {
        case '\r':
        case ' ':
            return INPUT_SELECT;

        case 27:
        case 'q':
        case 'Q':
            return INPUT_QUIT;
    }

    return INPUT_NONE;
}
