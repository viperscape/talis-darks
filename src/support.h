#ifndef SUPPORT_H
#define SUPPORT_H

#include "../SDL/SDL.h"

static inline int clicked(SDL_Event *event, SDL_Rect *rect) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;
        if ((x > rect->x) &&  (x < rect->x + rect->w)) {
            if ((y > rect->y) &&  (y < rect->y + rect->h)) {
                return 1;
            }
        }
    }

    return 0;
}

#endif