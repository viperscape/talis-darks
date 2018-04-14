#include <stdio.h>
#include "../SDL/SDL.h"
#include "ui.h"
#include "font.h"
#include "gfx.h"
#include "player.h"

int win_w = 1024;
int win_h = 768;
SDL_Renderer* render_ctx = NULL;
struct font font, font24;
struct game_screen menus;

struct player player = { 10, 95, 8, 75, 63, 55, 4, 30, 21, 50, 98 };

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
    else if (screen_get(&menus) == game_ui) {
        char stats[256] = {'\0'};
        sprintf(stats, "player %s\nstr: %i\nagility: %i\nintellect %i\ncharisma %i\n", 
            "Galendra", player.strength, player.agility, player.intellect, player.charisma);
        struct cache cache = font_cache(&font24, 50, 50, stats);
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
    SDL_Renderer* renderer = gfx_init("Talis Darks", 200, 200, win_w,win_h);
    font = font_init(renderer, "assets/fonts/constantia");
    font24 = font_init(renderer, "assets/fonts/constantia-24");

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