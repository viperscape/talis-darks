#ifndef SUPPORT_H
#define SUPPORT_H

#include "SDL.h"
#include "gfx.h"

static inline int clicked(SDL_Event *event, SDL_Rect *rect) {
    int win_w = 0, win_h = 0;
    SDL_GetWindowSize(gfx_get_window(), &win_w, &win_h);

    int x = 0, y = 0;

    if ((event->type == SDL_FINGERDOWN) || (event->type == SDL_MOUSEBUTTONDOWN)) {
        if (event->type == SDL_FINGERDOWN) {
            x = event->tfinger.x * win_w;
            y = event->tfinger.y * win_h;
        }
        else {
            x = event->button.x;
            y = event->button.y;
        }

        if ((x > rect->x) &&  (x < rect->x + rect->w)) {
            if ((y > rect->y) &&  (y < rect->y + rect->h)) {
                return 1;
            }
        }
    }

    return 0;
}

#endif