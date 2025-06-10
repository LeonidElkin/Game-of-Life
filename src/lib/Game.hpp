#pragma once

#include "utils.hpp"
#include <SDL.h>

class Game {
	static constexpr int screen_width = 800;
	static constexpr int screen_width_half = 400;
	static constexpr int screen_height = 600;
	static constexpr int screen_height_half = 300;
	static constexpr float cell_size = 40.0F;

	GameInfo info_;
	Camera cam_;
	SDL_Window *window_;
	SDL_Renderer *renderer_;

	[[nodiscard]] auto worldToScreen(float worldX, float worldY) const;

	void drawGrid();

public:
	explicit Game(const GameInfo &info);
	Game &operator=(const Game &) = delete;
	Game(const Game &) = delete;
	Game &operator=(Game &&) = delete;
	Game(Game &&) = delete;

	~Game();

	void run();
};
