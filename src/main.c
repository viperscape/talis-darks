#include <stdio.h>
#include "../SDL/SDL.h"
#include "ui.h"
#include "font.h"
#include "gfx.h"

int win_w = 1024;
int win_h = 768;
SDL_Renderer* render_ctx = NULL;
struct font font;
struct game_screen menus;

int handle_input () {
    SDL_Event event;
    while( SDL_PollEvent(&event) ) {
        if ((event.type == SDL_QUIT) ||
            ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_F12))) {
            return 1;
        }
        else if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) {
            screen_toggle(&menus, main_menu_ui);
        }
    }

    return 0;
}

int render_cycle (SDL_Renderer* renderer, float delta) {
    if (handle_input() == 1) return 1;

    if (screen_get(&menus) == main_menu_ui) {
        struct cache cache = font_cache(&font, 50, 50, "Talis Darks");
        cache_render(renderer, &cache);
        cache_free(&cache);
    }

    return 0;
}

void game_free() {
    gfx_free();
    font_free(&font);
}

SDL_Renderer* game_init () {
    SDL_Renderer* renderer = gfx_init(win_w,win_h);
    font = font_init(renderer, "assets/fonts/consolas");

    return renderer;
}

int main(int argc, char *argv[]) {
    menus = screen_init(game_ui);

    render_ctx = game_init();

    // render loop
    gfx_render(&render_cycle);

    game_free();

    return 0;
}