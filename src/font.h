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

struct text {
    int len;
    SDL_Rect bounds;
    SDL_Rect *src;
    SDL_Rect *dest;
    SDL_Texture *tex;
};

extern struct font font_init (SDL_Renderer* renderer, char *filename);
extern struct text font_build (struct font *font, int x, int y, char *str);
extern void text_render (SDL_Renderer* renderer, struct text *text);
extern void text_free (struct text *text);
extern void font_free (struct font *font);
extern int text_click(struct text *text, int x, int y);

#endif