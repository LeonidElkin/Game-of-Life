#pragma once

#include "utils.hpp"
#include <SDL.h>
#include <unordered_set>

class Game {
	static constexpr int default_screen_width = 800;
	static constexpr int default_screen_width_half = 400;
	static constexpr int default_screen_height = 600;
	static constexpr int default_screen_height_half = 300;
	static constexpr float cell_size = 40.0F;
	static constexpr uint8_t default_alpha = 255;

	struct PairHash {
		std::size_t operator()(const std::pair<int, int> &p) const {
			return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
		}
	};

	int screenWidth_ = default_screen_width;
	int screenWidthHalf_ = default_screen_width_half;
	int screenHeight_ = default_screen_height;
	int screenHeightHalf_ = default_screen_height_half;
	GameInfo info_;
	Camera cam_;
	SDL_Window *window_;
	SDL_Renderer *renderer_;
	std::unordered_set<std::pair<int, int>, PairHash> alive_cells_;

	[[nodiscard]] SDL_FRect worldToScreenCell(float worldX, float worldY) const;

	void drawGrid();
	void step();

public:
	explicit Game(const GameInfo &info);
	Game &operator=(const Game &) = delete;
	Game(const Game &) = delete;
	Game &operator=(Game &&) = delete;
	Game(Game &&) = delete;

	~Game();

	void run();
};
