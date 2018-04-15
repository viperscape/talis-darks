#include "font.h"
#include "../SDL/SDL.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct font font_init (SDL_Renderer* renderer, char *filename) {
    char filename_[256] = {'\0'};
    strcat(filename_, filename);
    strcat(filename_, ".fnt");

    struct ch chs[256] = {
        0,0,0,0,0,0,0,0
    };

    struct font font = { 0 , chs, NULL };

    FILE *f = fopen(filename_, "r");
    if (!f)  {
        fprintf(stderr, "font load error %s\n", filename_);
        exit(1);
    }

    char line[1024] = {'\0'};
    int c;
    int i = 0;
    int skiplines = 3;
    int count = 0;
    //int cursor = 0;

    while ((c = fgetc(f)) != EOF) {
        if (i == 1023) {
            fprintf(stderr, "max characters exceeded\n");
            i = 0;
            continue;
        }

        if ((c == '\n') || (c == '\r')) {
            if (skiplines > 0) skiplines--;
            else {
                int rc = 0;
                if (count < 1) {
                    rc = sscanf(line, "chars count=%i", &count);
                    if (rc < 1) fprintf(stderr, "font parse error %s\n", line);
                    font.count = count;
                }
                else {
                    int id, x, y, w, h, xo, yo, xa, p, chl = 0;
                    rc = sscanf(line, "char id=%i   x=%i    y=%i     width=%i     height=%i    xoffset=%i     yoffset=%i     xadvance=%i     page=%i  chnl=%i", &id, &x, &y, &w, &h, &xo, &yo, &xa, &p, &chl);
                    if (rc < 10) fprintf(stderr, "font parse error %s\n", line);
                    struct ch ch = {id, x, y, w, h, xo, yo, xa};
                    font.chs[id] = ch;
                    //cursor++;
                }
            }

            for (i;i>0;--i) line[i] = '\0';
        }
        else {
            line[i] = c;
            i++;
        }
    }

    fclose(f);

    // load texture
    for (int i=0; i<256; ++i) filename_[i] = '\0';
    strcat(filename_, filename);
    strcat(filename_, ".bmp");
    SDL_Surface* surface = NULL;
    surface = SDL_LoadBMP(filename_);
    if (surface == NULL) {
        printf("Could not create surface: %s\n\t%s\n", filename_, SDL_GetError());
    }

    font.tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = NULL;

    return(font);
}

struct cache font_cache (struct font *font, int x, int y, char *str) {
    int cx = x;
    int cy = y; //cursors
    char ch;

    struct cache cache = { 0, { x, y, 0,0 }, NULL, NULL, font->tex };
    
    for(int i = 0; i<strlen(str); ++i) {
        ch = str[i];
        if (ch == ' ') {
            cx += font->chs['j'].xadv; // note: we use static 'j' element
            continue;
        }
        else if (ch == '\t') {
            cx += font->chs['j'].xadv * 4; // static 4 space tab size
            continue;
        }
        else if (ch == '\n') {
            cx = x;
            cy += font->chs['j'].h + 2;
            continue;
        }

        if (cx - x > cache.bounds.w) cache.bounds.w = cx + font->chs['j'].xadv - x;
        cache.bounds.h = (cy - y) + font->chs['j'].h + 2;
        
        SDL_Rect *src = realloc(cache.src, sizeof(SDL_Rect) * (cache.len + 1));
        SDL_Rect *dest = realloc(cache.dest, sizeof(SDL_Rect) * (cache.len + 1));
        if ((src == NULL)||(dest == NULL)) {
            fprintf(stderr, "exceeded dynamic mem\n");
            exit(1);
        }

        cache.src = src;
        cache.dest = dest;

        cache.src[cache.len].x = font->chs[ch].x;
        cache.src[cache.len].y = font->chs[ch].y;
        cache.src[cache.len].w = font->chs[ch].w;
        cache.src[cache.len].h = font->chs[ch].h;

        cache.dest[cache.len].x = cx + font->chs[ch].xoff;
        cache.dest[cache.len].y = cy + font->chs[ch].yoff;
        cache.dest[cache.len].w = font->chs[ch].w;
        cache.dest[cache.len].h = font->chs[ch].h;
        cx += font->chs[ch].xadv;

        cache.len++;
    }

    return cache;
}

void cache_render (SDL_Renderer* renderer, struct cache *cache) {
    /* render area for debug
    SDL_Surface *s = SDL_CreateRGBSurface(0, cache->bounds.w, cache->bounds.h, 32, 0, 0, 0, 0);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 255, 0, 0));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    SDL_RenderCopy(renderer, tex, NULL, &cache->bounds);
    SDL_DestroyTexture(tex);
    */

    for (int i = 0; i < cache->len; ++i)
        SDL_RenderCopy(renderer, cache->tex, &cache->src[i], &cache->dest[i]);
}

void cache_free (struct cache *cache) {
    if (cache->dest == NULL) return;

    free(cache->dest);
    free(cache->src);
    cache->dest = NULL;
    cache->src = NULL;
}

void font_free (struct font *font) {
    if (font->tex == NULL) return;
    SDL_DestroyTexture(font->tex);
    font->tex = NULL;
}

int cache_click(struct cache *cache, int x, int y) {
    if ((x > cache->bounds.x) &&  (x < cache->bounds.x + cache->bounds.w)) {
        if ((y > cache->bounds.y) &&  (y < cache->bounds.y + cache->bounds.h)) {
            return 1;
        }
    }

    return 0;
}