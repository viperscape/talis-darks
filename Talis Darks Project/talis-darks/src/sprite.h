#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"

enum sprite_heading {
    heading_N, heading_NE, heading_E, 
    heading_SE, heading_S, heading_SW, 
    heading_W, heading_NW,
    heading_None
};

struct sprite {
    int id;
    int cx;
    int cy;
    enum sprite_heading heading;
    int hide;
    SDL_Rect rect;
    SDL_Texture *tex[9];
};

struct sprites {
    struct sprite *group;
    int num;
};

extern struct sprite sprite_init ();
extern struct sprite sprite_copy(struct sprite *s);
extern struct sprite sprite_load (SDL_Renderer* renderer, char* filename, enum sprite_heading);
extern void sprite_render (SDL_Renderer* renderer, struct sprite *s);
extern void sprite_free(struct sprite *s);

extern int sprites_add(struct sprites *g, struct sprite *s);
extern void sprites_free(struct sprites *g);

extern int sprite_click(SDL_Event *event, struct sprite *s);

#endif