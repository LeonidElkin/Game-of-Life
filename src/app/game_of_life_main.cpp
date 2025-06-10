#include "Game.hpp"
#include "utils.hpp"
#include <SDL.h>
#include <iostream>

int main(int argc, char **argv) {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
		return 1;
	}

	Game(parseOptions(argc, argv)).run();

	SDL_Quit();

	return 0;
}
