#ifndef FONT_H
#define FONT_H

#include "../SDL/SDL.h"

struct ch {
    char id;
    int x, y;
    int w, h;
    int xoff, yoff;
    int xadv;
};

struct font {
    int count;
    struct ch chs[256];
    SDL_Texture *tex;
};

struct cache {
    int len;
    SDL_Rect *src;
    SDL_Rect *dest;
    SDL_Texture *tex;
};

extern struct font font_init (SDL_Renderer* renderer, char *filename);
extern void font_render (SDL_Renderer* renderer, struct font *font, int x, int y, char *str);
extern struct cache font_cache (struct font *font, int x, int y, char *str);
extern void cache_render (SDL_Renderer* renderer, struct cache *cache);
extern void cache_free (struct cache *cache);
extern void font_free (struct font *font);

#endif