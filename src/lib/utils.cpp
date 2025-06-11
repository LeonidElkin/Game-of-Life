#include "utils.hpp"

#include <cxxopts.hpp>
#include <iostream>

GameInfo parseOptions(int argc, char **argv) {
	cxxopts::Options options(
			"gol", "Conway's Game of Life SDL2 implementation.\n\nClick the left mouse button to add/remove live "
				   "cells. Click space to pause/resume the game. Hold the middle mouse button to move the camera. "
				   "Scroll the mousewheel to zoom");

	options.add_options()("s, speed", "Speed of the game in ms",
						  cxxopts::value<uint32_t>()->default_value("500"))("h, help", "Show help");

	auto result = options.parse(argc, argv);

	if (result.count("help")) {
		std::cout << options.help() << '\n';
		exit(0);
	}

	return {.speed = result["speed"].as<uint32_t>()};
}