#ifndef UI_H
#define UI_H

#include <stdlib.h>

enum ui_screens { game_ui, main_menu_ui, unit_ui };
struct game_screen {
    enum ui_screens* menu_state;
    int screen_num;
    int screen_cap;
};

extern struct game_screen screen_init ();
extern void screen_free (struct game_screen *screen);
extern void screen_add (struct game_screen *screen, enum ui_screens ui);
extern enum ui_screens screen_get (struct game_screen *screen);
extern void screen_remove (struct game_screen *screen);
extern int screen_removeif (struct game_screen *screen, enum ui_screens ui);
extern int screen_toggle (struct game_screen *screen, enum ui_screens ui);

#endif