#include "SDL.h"

#include "support.h"
#include <stdio.h>
#include "ui.h"
#include "font.h"
#include "gfx.h"
#include "player.h"

int win_w = 500;
int win_h = 800;
SDL_Renderer* render_ctx = NULL;
struct font font, font36;
struct game_screen menus;
struct text mainmenu_text;
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
            if (clicked(&event, &mainmenu_text.bounds)) screen_toggle(&menus, main_menu_ui);

            int rc = clicked(&event, &stat_text.bounds);
            //if (rc) printf("clicked stat_text\n");
			int i = 0;
			for (i; i < 4; ++i) {
                int rc = clicked(&event, &stat_bounds[i]);
                //if (rc) printf("clicked stat: %i\n", i);
            }
        }
        else if (screen_get(&menus) == main_menu_ui) {
            if (clicked(&event, &title_text.bounds)) screen_toggle(&menus, main_menu_ui);
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
        SDL_SetTextureColorMod(mainmenu_text.tex, 255, 205, 0);
        text_render(renderer, &mainmenu_text);

        SDL_SetTextureColorMod(stat_text.tex, 200, 200, 200);
        text_render(renderer, &stat_text);

        // render stats
        int alignx = stat_text.bounds.w + stat_text.bounds.x + 50;
        int aligny = stat_text.bounds.y;
        char buf[256] = {'\0'};

        SDL_Rect text_bounds = text_draw(renderer, &font36, alignx, aligny, "Galendra", 230, 255, 230);

        int stats[4] = { player.strength, player.agility, player.intellect, player.charisma };

		int i = 0;
        Uint8 r = 0, g = 0, b = 0;
        for (i;i<4;++i) {
            sprintf(buf, "%i", stats[i]);
            int next_y = aligny + (text_bounds.h * (i + 1));
            
            if (stats[i] > 85) r = 200, g = 240, b = 255;
            else if (stats[i] < 20) r = 255, g = 64, b = 64;
            else r = 255, g = 240, b = 200;

            stat_bounds[i] = text_draw(renderer, &font36, alignx, next_y, buf, r, g, b);
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
    font36 = font_init(renderer, "fonts/constantia-36");

    mainmenu_text = font_build(&font36, 5, 5, "Options");
    title_text = font_build(&font, 50, 50, "Talis Darks");
    stat_text = font_build(&font36, 50, 50, "player\nstrength\nagility\nintellect\ncharisma");

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