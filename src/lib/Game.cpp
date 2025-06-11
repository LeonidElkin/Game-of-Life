#include "Game.hpp"
#include "utils.hpp"
#include <SDL.h>
#include <cmath>

SDL_FRect Game::worldToScreenCell(float worldX, float worldY) const {
	auto const screenX = (worldX - cam_.x) * cam_.zoom + screen_width_half;
	auto const screenY = (worldY - cam_.y) * cam_.zoom + screen_height_half;

	return {screenX, screenY, cell_size * cam_.zoom, cell_size * cam_.zoom};
}

void Game::drawGrid() {
	constexpr uint8_t default_border_color = 128;
	constexpr uint8_t default_alive_color = 255;

	auto const left = cam_.x - screen_width_half / cam_.zoom;
	auto const right = cam_.x + screen_width_half / cam_.zoom;
	auto const top = cam_.y - screen_height_half / cam_.zoom;
	auto const bottom = cam_.y + screen_height_half / cam_.zoom;

	int const leftCell = static_cast<int>(floor(left / cell_size));
	int const rightCell = static_cast<int>(ceil(right / cell_size));
	int const topCell = static_cast<int>(floor(top / cell_size));
	int const bottomCell = static_cast<int>(ceil(bottom / cell_size));

	SDL_SetRenderDrawColor(renderer_, default_border_color, default_border_color, default_border_color, default_alpha);

	for (int itx = leftCell; itx <= rightCell; ++itx) {
		for (int ity = topCell; ity <= bottomCell; ++ity) {
			auto const rect =
					worldToScreenCell(static_cast<float>(itx) * cell_size, static_cast<float>(ity) * cell_size);
			if (alive_cells_.contains(std::make_pair(itx, ity))) {
				SDL_SetRenderDrawColor(renderer_, default_alive_color, default_alive_color, default_alive_color,
									   default_alpha);
				SDL_RenderFillRectF(renderer_, &rect);
			}
			SDL_SetRenderDrawColor(renderer_, default_border_color, default_border_color, default_border_color,
								   default_alpha);
			SDL_RenderDrawRectF(renderer_, &rect);
		}
	}
}

Game::Game(const GameInfo &info)
	: info_(info), cam_(Camera()),
	  window_(SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width,
							   screen_height, SDL_WINDOW_SHOWN)),
	  renderer_(SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED)) {}

Game::~Game() {
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

void Game::run() {
	constexpr uint8_t default_background_color = 0;
	const auto zoomFactorIncrease = 1.1F;
	const auto zoomFactorDecrease = 0.9F;
	const auto delay = 16;

	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_MOUSEWHEEL:
					cam_.zoom *= (event.wheel.y > 0) ? zoomFactorIncrease : zoomFactorDecrease;
					break;
				case SDL_MOUSEMOTION:
					if (event.motion.state & SDL_BUTTON_MMASK) {
						cam_.x -= static_cast<float>(event.motion.xrel) / cam_.zoom;
						cam_.y -= static_cast<float>(event.motion.yrel) / cam_.zoom;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						auto worldX = static_cast<float>(event.button.x - screen_width_half) / cam_.zoom + cam_.x;
						auto worldY = static_cast<float>(event.button.y - screen_height_half) / cam_.zoom + cam_.y;
						int const cellX = static_cast<int>(std::floor(worldX / cell_size));
						int const cellY = static_cast<int>(std::floor(worldY / cell_size));
						auto const cell = std::make_pair(cellX, cellY);

						if (alive_cells_.contains(cell)) {
							alive_cells_.erase(cell);
						} else {
							alive_cells_.insert(cell);
						}
					}
					break;
				default:
					break;
			}
		}

		SDL_SetRenderDrawColor(renderer_, default_background_color, default_background_color, default_background_color,
							   default_alpha);
		SDL_RenderClear(renderer_);

		drawGrid();

		SDL_RenderPresent(renderer_);
		SDL_Delay(delay);
	}
}
