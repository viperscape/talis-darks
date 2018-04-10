#include <stdio.h>

#include "ui.h"

int main() {
    struct game_screen ui = screen_init(main_menu_ui);

    int c = -1;
    for(;;) {
        printf("====\n");
        switch (screen_get(&ui)) {
            case game_ui:
                if (c == ' ') screen_add(&ui, unit_ui);
                else if (c == 'b') screen_remove(&ui);
                else {
                    printf("entering game\npress q to quit\n");
                    printf("unit: space\n");
                    printf("back: b\n");
                }

                break;
            
            case main_menu_ui:
                if (c == ' ') screen_add(&ui, game_ui);
                else {
                    printf("the choice is yours\n");
                    printf("start game: space\n");
                    printf("end game: q\n");
                }
                
                break;

            default:
                printf("unhandled menu screen %i\n", screen_get(&ui));
                screen_remove(&ui);
                break;
        }

        c = -1;
        if ((c = getchar()) == 'q') break;
    }
}