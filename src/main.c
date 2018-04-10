#include <stdio.h>

#include "ui.h"

int main() {
    struct game_screen ui = screen_init(main_menu_ui);

    int c;
    while ((c = getchar()) != 'q') {
        if (c == '\n') continue;

        switch (screen_get(&ui)) {
            case game_ui: 
                printf("enter game\n");
                break;
            
            case main_menu_ui:
                printf("the choice is yours\n");
                break;

            default:
                printf("unhandled menu screen %i\n", screen_get(&ui));
                break;
        }
    }
}