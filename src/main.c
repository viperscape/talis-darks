#include <stdio.h>

#include "ui.h"

int main() {
    struct game_screen ui = screen_init();

    int c;
    while ((c = getchar()) != 'x') {
        if (c == '\n') continue;
        printf("ui %i\n", screen_get(&ui));
    }
}