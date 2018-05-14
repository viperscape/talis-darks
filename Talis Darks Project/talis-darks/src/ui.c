#include "ui.h"

struct game_screen screen_init (enum ui_screens menu) {
    enum ui_screens* menus = malloc(sizeof(enum ui_screens) * 1);
    menus[0] = menu;

    struct game_screen screen = { menus, 1, 1 };
    return screen;
}

void screen_free (struct game_screen *screen) {
    if (screen->menu_state == NULL) return;

    free(screen->menu_state);
    screen->menu_state = NULL;
    screen->screen_cap = 0;
    screen->screen_num = 0;
}

enum ui_screens screen_get (struct game_screen *screen) {
    return screen->menu_state[screen->screen_num-1];
}

/// adds screen only once
void screen_add (struct game_screen *screen, enum ui_screens ui) {
    if (screen_get(screen) == ui) return;

    if (screen->screen_num == screen->screen_cap) {
        enum ui_screens* menus_ = realloc(screen->menu_state, sizeof(enum ui_screens) * (screen->screen_num + 1));   
        if (menus_ == NULL) return;

        screen->menu_state = menus_;
        screen->screen_cap += 1;
    }

    screen->menu_state[screen->screen_num] = ui;
    screen->screen_num += 1;
}



void screen_remove (struct game_screen *screen) {
    screen->screen_num -= 1;
}

int screen_removeif (struct game_screen *screen, enum ui_screens ui) {
    if (screen_get(screen) == ui) {
        screen_remove(screen);
        return 1;
    }

    return 0;
}

int screen_toggle (struct game_screen *screen, enum ui_screens ui) {
    int toggle = screen_removeif(screen, ui);
    if (toggle < 1) screen_add(screen, ui);

    return toggle < 1;
}