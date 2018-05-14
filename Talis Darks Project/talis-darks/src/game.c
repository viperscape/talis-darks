#include "SDL.h"

#include "support.h"
#include <stdio.h>
#include "ui.h"
#include "font.h"
#include "gfx.h"
#include "player.h"

int win_w = 1024;
int win_h = 768;
SDL_Renderer* render_ctx = NULL;
struct font font, font24;
struct game_screen menus;
struct text stat_text;
struct text title_text;
SDL_Rect stat_bounds[4] = {{0,0,0,0}};

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

        if (screen_get(&menus) == game_ui) {
            int rc = clicked(&event, &stat_text.bounds);
            //if (rc) printf("clicked stat_text\n");
			int i = 0;
			for (i; i < 4; ++i) {
                int rc = clicked(&event, &stat_bounds[i]);
                //if (rc) printf("clicked stat: %i\n", i);
            }
        }
    }

    return 0;
}

int render_cycle (SDL_Renderer* renderer, float delta) {
    if (handle_input() == 1) return 1;

    if (screen_get(&menus) == main_menu_ui) {
        SDL_SetTextureColorMod(title_text.tex, 255, 205, 0);
        text_render(renderer, &title_text);
    }
    else if (screen_get(&menus) == game_ui) {
        SDL_SetTextureColorMod(stat_text.tex, 200, 200, 200);
        text_render(renderer, &stat_text);

        // render stats
        struct text text;
        int alignx = stat_text.bounds.w + stat_text.bounds.x + 50;
        int aligny = stat_text.bounds.y;
        char buf[256] = {'\0'};

        text = font_build(&font24, alignx, aligny, "Galendra");
        SDL_SetTextureColorMod(text.tex, 230, 255, 230);
        text_render(renderer, &text);
        text_free(&text);

        int stats[4] = { player.strength, player.agility, player.intellect, player.charisma };

		int i = 0;
        for (i;i<4;++i) {
            sprintf(buf, "%i", stats[i]);
            text = font_build(&font24, alignx, aligny+(text.bounds.h * (i + 1)), buf);
            stat_bounds[i] = text.bounds;
            if (stats[i] > 85) SDL_SetTextureColorMod(text.tex, 200, 240, 255);
            else if (stats[i] < 20) SDL_SetTextureColorMod(text.tex, 255, 64, 64);
            else SDL_SetTextureColorMod(text.tex, 255, 240, 200);
            text_render(renderer, &text);
            text_free(&text);
        }
    }

    return 0;
}

void game_free() {
    gfx_free();
    text_free(&stat_text);
    text_free(&title_text);
    font_free(&font);
}

SDL_Renderer* game_init () {
    SDL_Renderer* renderer = gfx_init("Talis Darks", 200, 200, win_w,win_h);
    font = font_init(renderer, "fonts/constantia");
    font24 = font_init(renderer, "fonts/constantia-24");

    title_text = font_build(&font, 50, 50, "Talis Darks");
    stat_text = font_build(&font24, 50, 50, "player\nstrength\nagility\nintellect\ncharisma");

    return renderer;
}

void game_start() {
    menus = screen_init(game_ui);

    render_ctx = game_init();

    // render loop
    gfx_render(&render_cycle);

    game_free();

    SDL_Delay(1500);
}