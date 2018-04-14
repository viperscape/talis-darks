#include "sprite.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct sprite sprite_init() {
    struct sprite s = { 0,0,0, heading_E, 0, {0,0,0,0} , {NULL} };
    return s;
}

/// copies sprite, except for position/id
struct sprite sprite_copy(struct sprite *s) {
    struct sprite s_ = sprite_init();
    s_.rect.w = s->rect.w;
    s_.rect.h = s->rect.h;
    s_.heading = s->heading;
    s_.hide = s->hide;
    for (int i = 0; i < 9; ++i)
        s_.tex[i] = s->tex[i]; // copies only texture pointer itself
    return s_;
}

void sprite_render (SDL_Renderer* renderer, struct sprite *s) {
    SDL_RenderCopy(renderer, s->tex[s->heading], NULL, &s->rect);
}


struct sprite sprite_load (SDL_Renderer* renderer, char* filename, enum sprite_heading heading) {
    struct sprite s = sprite_init();
    s.heading = heading;

    char* ext = ".bmp";
    char filename_[256] = {'\0'};
    printf("loading sprite %s\n", filename);

    SDL_Surface* surface = NULL;
    for(int i = 0; i < 8; ++i) {
        strcpy(filename_, filename);
        if (heading != heading_None) {
            if (i == 0) strcat(filename_, "_N");
            if (i == 1) strcat(filename_, "_NE");
            if (i == 2) strcat(filename_, "_E");
            if (i == 3) strcat(filename_, "_SE");
            if (i == 4) strcat(filename_, "_S");
            if (i == 5) strcat(filename_, "_SW");
            if (i == 6) strcat(filename_, "_W");
            if (i == 7) strcat(filename_, "_NW");
        }

        strcat(filename_, ext);

        surface = SDL_LoadBMP(filename_);
        if (surface == NULL) {
            printf("Could not create surface: %s\n", SDL_GetError());
            return s;
        }
        
        Uint32 key = SDL_MapRGB(surface->format, 255, 0, 255);
        int r = SDL_SetColorKey(surface, SDL_TRUE, key);
        if (r > 0) {
            printf("Could not key surface: %s\n", SDL_GetError());
            return s;
        }


        s.tex[i] = SDL_CreateTextureFromSurface(renderer, surface);
        s.rect.w = surface->w;
        s.rect.h = surface->h;

        if (heading == heading_None) {
            s.tex[heading_None] = s.tex[0]; // set default texture to first loaded
            break;
        }
    }

    SDL_FreeSurface(surface);
    surface = NULL;

    return s;
}

void sprite_free(struct sprite *s) {
    if (s->tex[0] != NULL) {
        for(int i = 0; i < 9; ++i) {
            SDL_DestroyTexture(s->tex[i]);
            s->tex[i] = NULL;
        }
    }
}

void sprites_free(struct sprites *g) {
    for (g->num; g->num > 0; g->num--) {
        sprite_free(&g->group[g->num-1]);
    }
    free(g->group);
}

int sprites_add(struct sprites *g, struct sprite *s) {
    struct sprite *sprites_ = realloc(g->group, sizeof(struct sprite) * (g->num+1));
    if (sprites_ == NULL) return;
    sprites_[g->num] = sprite_copy(s);
    g->num += 1;
    g->group = sprites_;

    return g->num-1;
}

int sprite_click(struct sprite *s, int x, int y) {
    if ((x > s->rect.x) &&  (x < s->rect.x + s->rect.w)) {
        if ((y > s->rect.y) &&  (y < s->rect.y + s->rect.h)) {
            return 1;
        }
    }

    return 0;
}