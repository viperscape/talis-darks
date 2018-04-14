#include "../SDL/SDL.h"

extern SDL_Renderer* gfx_init(char *title, int x, int y, int w, int h);
extern int gfx_render(int (cb)(SDL_Renderer* renderer, float delta));
extern void gfx_free();
extern SDL_Window* gfx_get_window();