#include "utils.hpp"

#include <cxxopts.hpp>
#include <iostream>
#include <magic_enum/magic_enum.hpp>

GameInfo parseOptions(int argc, char **argv) {
	cxxopts::Options options("gol", "Conway's Game of Life SDL2 implementation");

	options.add_options()("gt,game_type", "Game of Life type: 0 stands for bordered, 1 stands for closure",
						  cxxopts::value<uint8_t>()->default_value("0"))(
			"w,width", "Width of the grid in blocks", cxxopts::value<uint32_t>()->default_value("1000"))(
			"h,height", "Height of the grid in blocks", cxxopts::value<uint32_t>()->default_value("800"));

	auto result = options.parse(argc, argv);

	if (result.count("help") != 0U) {
		std::cout << options.help() << '\n';
		exit(0);
	}

	auto type = result["game_type"].as<uint8_t>();
	if (!magic_enum::enum_contains<GameType>(type)) {
		std::cout << "Incorrect game type. Use --help to check out info\n" << '\n';
		exit(0);
	}

	return {.width=result["width"].as<uint32_t>(), .height=result["height"].as<uint32_t>(), .type=static_cast<GameType>(type)};
}