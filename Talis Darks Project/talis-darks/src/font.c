#include "font.h"
#include "SDL.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct font font_init (SDL_Renderer* renderer, char *filename) {
    char filename_[256] = {'\0'};
    strcat(filename_, filename);
    strcat(filename_, ".fnt");

    struct ch chs[256] = {
        0,0,0,0,0,0,0,0
    };

    struct font font = { 0 , chs, NULL };

	SDL_RWops *rw = SDL_RWFromFile(filename_, "rb");
    if (rw == NULL)  {
        SDL_Log("font load error %s\n", filename_);
        return font;
    }

    char line[1024] = {'\0'};
	char c = '\0';
    int i = 0;
    int skiplines = 3;
    int count = 0;

    while (SDL_RWread(rw, &c, 1, 1) > 0) {
        if (i == 1023) {
            SDL_Log("max characters exceeded\n%s\n", line);
            i = 0;
            continue;
        }

        if (c == '\n') {
            if (skiplines > 0) skiplines--;
            else {
                int rc = 0;
                if (count < 1) {
                    rc = sscanf(line, "chars count=%i", &count);
                    if (rc < 1) SDL_Log("font parse error %i %s\n", rc, line);
                    else font.count = count;
                }
                else {
                    int id, x, y, w, h, xo, yo, xa, p, chl = 0;
                    rc = sscanf(line, "char id=%i   x=%i    y=%i     width=%i     height=%i    xoffset=%i     yoffset=%i     xadvance=%i     page=%i  chnl=%i", &id, &x, &y, &w, &h, &xo, &yo, &xa, &p, &chl);
                    if (rc < 10) SDL_Log("font parse error %i %s\n", rc, line);
					else {
						struct ch ch = { id, x, y, w, h, xo, yo, xa };
						font.chs[id] = ch;
					}
                }
            }

            for (i;i>0;--i) line[i] = '\0';
        }
        else if (c != '\r') {
            line[i] = c;
            i++;
        }
    }

	SDL_RWclose(rw);

    // load texture
    i = 0;
    for (i; i<256; ++i) filename_[i] = '\0';
    strcat(filename_, filename);
    strcat(filename_, ".bmp");
    SDL_Surface* surface = NULL;
    surface = SDL_LoadBMP(filename_);
    if (surface == NULL) {
        SDL_Log("Could not create surface: %s\n\t%s\n", filename_, SDL_GetError());
    }

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
    font.tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = NULL;

    return(font);
}

struct text font_build (struct font *font, int x, int y, char *str) {
    int cx = x;
    int cy = y; //cursors
    char ch;

    struct text text = { 0, { x, y, 0,0 }, NULL, NULL, font->tex };
    int i = 0;
    for(i; i<strlen(str); ++i) {
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

        if (cx - x > text.bounds.w) text.bounds.w = (cx + font->chs['j'].w + font->chs['j'].xadv + 2) - x;
        text.bounds.h = (cy - y) + font->chs['j'].h + 2;
        
        SDL_Rect *src = realloc(text.src, sizeof(SDL_Rect) * (text.len + 1));
        SDL_Rect *dest = realloc(text.dest, sizeof(SDL_Rect) * (text.len + 1));
        if ((src == NULL)||(dest == NULL)) {
            SDL_Log("exceeded dynamic mem\n");
            exit(1);
        }

        text.src = src;
        text.dest = dest;

        text.src[text.len].x = font->chs[ch].x;
        text.src[text.len].y = font->chs[ch].y;
        text.src[text.len].w = font->chs[ch].w;
        text.src[text.len].h = font->chs[ch].h;

        text.dest[text.len].x = cx + font->chs[ch].xoff;
        text.dest[text.len].y = cy + font->chs[ch].yoff;
        text.dest[text.len].w = font->chs[ch].w;
        text.dest[text.len].h = font->chs[ch].h;
        cx += font->chs[ch].xadv;

        text.len++;
    }

    return text;
}

void text_render (SDL_Renderer* renderer, struct text *text) {
    // render area for debug
    /*
    SDL_Surface *s = SDL_CreateRGBSurface(0, text->bounds.w, text->bounds.h, 32, 0, 0, 0, 0);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 255, 0, 0));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    SDL_RenderCopy(renderer, tex, NULL, &text->bounds);
    SDL_DestroyTexture(tex);
    */
    int i = 0;
    for (i; i < text->len; ++i)
        SDL_RenderCopy(renderer, text->tex, &text->src[i], &text->dest[i]);
}

void text_free (struct text *text) {
    if (text->dest == NULL) return;

    free(text->dest);
    free(text->src);
    text->dest = NULL;
    text->src = NULL;
}

void font_free (struct font *font) {
    if (font->tex == NULL) return;
    SDL_DestroyTexture(font->tex);
    font->tex = NULL;
}

int text_click(struct text *text, int x, int y) {
    if ((x > text->bounds.x) &&  (x < text->bounds.x + text->bounds.w)) {
        if ((y > text->bounds.y) &&  (y < text->bounds.y + text->bounds.h)) {
            return 1;
        }
    }

    return 0;
}

/// render text on the fly, returns next pixels
SDL_Rect text_draw(SDL_Renderer* renderer, struct font *font, int x, int y, char *str, Uint8 r, Uint8 g, Uint8 b) {
    struct text text = font_build(font, x, y, str);
    SDL_Rect bounds = text.bounds; // NOTE we may want to add xadvance here
    SDL_SetTextureColorMod(text.tex, r, g, b);
    text_render(renderer, &text);
    text_free(&text);
    return bounds;
}