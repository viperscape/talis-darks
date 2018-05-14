#ifndef SUPPORT_H
#define SUPPORT_H

#include "SDL.h"

#if defined(__ANDROID__)
#include <android/log.h>
#define printd(...)  __android_log_print(ANDROID_LOG_DEBUG, "DBG", __VA_ARGS__)
#endif

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