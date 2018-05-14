#include "SDL.h"

extern "C" {
#include "game.h"
}

int main(int argc, char** argsv) {
	game_start();
	return 0;
}