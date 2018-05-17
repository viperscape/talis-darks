#include "SDL.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* native = NULL;
SDL_Texture* texture = NULL;

SDL_Window* gfx_get_window() { return window; }

SDL_Renderer* gfx_init(char *title, int x, int y, int w, int h) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title, 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        w, h, SDL_WINDOW_OPENGL);

    if (window==NULL) {   
        //printf("Could not create window: %s\n", SDL_GetError());
        return NULL;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer==NULL) {   
        //printf("Could not create renderer: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_RenderSetLogicalSize(renderer, w,h);
    native = SDL_GetWindowSurface(window);

    /*SDL_Surface* surface = NULL;
    surface = SDL_LoadBMP("assets/bg.bmp");
    if (surface == NULL) {
        printf("Could not create surface: %s\n", SDL_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = NULL;*/

    return renderer;
}

int gfx_render(int (cb)(SDL_Renderer* renderer, float delta)) {
    if (renderer == NULL) return 1;
    
    long time_last = SDL_GetTicks();
    float time_delta = 0.0;

    int quit = 0;
    while(!quit) {
        time_delta = (float)(SDL_GetTicks() - time_last);
        time_last = SDL_GetTicks();


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        //SDL_RenderCopy(renderer, texture, NULL, NULL);

        quit = cb(renderer, time_delta);

        SDL_RenderPresent(renderer);

        if (time_delta < 15.0) { // cap at 60fps/15ms min frame time
            SDL_Delay((long)(15.0 - time_delta));
        }
    }

    return 0;
}

void gfx_free() {
    if (window != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;

        SDL_FreeSurface(native);
        native = NULL;

        SDL_DestroyRenderer(renderer);
        renderer = NULL;

        SDL_DestroyWindow(window);
        window = NULL;

        SDL_Quit();
    }
}