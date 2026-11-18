#include <input.h>
#include <platform.h>
#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

static HANDLE console_input = INVALID_HANDLE_VALUE;
static HANDLE console_output = INVALID_HANDLE_VALUE;
static DWORD original_output_mode;
static bool output_mode_saved = false;
static CONSOLE_CURSOR_INFO original_cursor;
static bool cursor_saved = false;

static HANDLE input_handle(void) {
    if (console_input == INVALID_HANDLE_VALUE || console_input == NULL) {
        console_input = CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                                    OPEN_EXISTING, 0, NULL);
        if (console_input == INVALID_HANDLE_VALUE) {
            console_input = GetStdHandle(STD_INPUT_HANDLE);
        }
    }

    return console_input;
}

void input_init(void) {
    input_handle();

    console_output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (console_output == INVALID_HANDLE_VALUE || console_output == NULL) {
        return;
    }

    if (GetConsoleMode(console_output, &original_output_mode)) {
        DWORD mode = original_output_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        output_mode_saved = SetConsoleMode(console_output, mode) != 0;
    }
}

void input_shutdown(void) {
    if (output_mode_saved) {
        SetConsoleMode(console_output, original_output_mode);
        output_mode_saved = false;
    }
}

uint32_t platform_ticks_ms(void) { return (uint32_t)GetTickCount(); }

void platform_cursor_hide(void) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    if (output == INVALID_HANDLE_VALUE || output == NULL) {
        return;
    }

    if (!GetConsoleCursorInfo(output, &info)) {
        return;
    }

    original_cursor = info;
    cursor_saved = true;

    info.bVisible = FALSE;
    SetConsoleCursorInfo(output, &info);
}

void platform_cursor_show(void) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!cursor_saved || output == INVALID_HANDLE_VALUE || output == NULL) {
        return;
    }

    SetConsoleCursorInfo(output, &original_cursor);
    cursor_saved = false;
}

static bool read_key(KEY_EVENT_RECORD *key) {
    HANDLE handle = input_handle();
    INPUT_RECORD record;
    DWORD count = 0;

    for (;;) {
        if (!GetNumberOfConsoleInputEvents(handle, &count) || count == 0) {
            Sleep(100);
            return false;
        }

        if (!ReadConsoleInputW(handle, &record, 1, &count) || count != 1) {
            return false;
        }

        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
            *key = record.Event.KeyEvent;
            return true;
        }
    }
}

Input input_poll(void) {
    KEY_EVENT_RECORD key;

    if (!read_key(&key)) {
        return INPUT_NONE;
    }

    switch (key.wVirtualKeyCode) {
        case 'W':
            return INPUT_UP;
        case 'S':
            return INPUT_DOWN;
        case 'A':
            return INPUT_LEFT;
        case 'D':
            return INPUT_RIGHT;
        case VK_UP:
            return INPUT_UP;
        case VK_DOWN:
            return INPUT_DOWN;
        case VK_LEFT:
            return INPUT_LEFT;
        case VK_RIGHT:
            return INPUT_RIGHT;
        case VK_RETURN:
        case VK_SPACE:
            return INPUT_SELECT;
        case 'Q':
            return INPUT_QUIT;
    }

    return INPUT_NONE;
}
