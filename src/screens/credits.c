#include <input.h>
#include <screen.h>
#include <stdint.h>
#include <stdio.h>

static Screen credits_run(void) {
    static const char authors[] = "@Craft3324 (Nobel)\n"
                                  "@xxxx      (Leo)\n"
                                  "@Andrija97 (Andrija)\n"
                                  "@iLavion   (Edvin)\n";
    bool running = true;

    while (running) {
        printf("\033[2J\033[H");

        printf("\nCredits\n\n");
        printf("%s\n", authors);

        printf(BG BLACK " > %-20s " RESET "\n", "Go back");

        fflush(stdout);
        switch (input_poll()) {
            case INPUT_SELECT:
                running = false;
                break;
            default:
                break;
        };
    }

    return SCREEN_MENU;
}

ScreenHandler credits = {.run = credits_run};
