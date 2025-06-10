#include "Game.hpp"
#include "utils.hpp"
#include <SDL.h>
#include <cmath>

auto Game::worldToScreen(float worldX, float worldY) const {
	SDL_Rect rect;
	auto const screenX = (worldX - cam_.x) * cam_.zoom + screen_width_half;
	auto const screenY = (worldY - cam_.y) * cam_.zoom + screen_height_half;

	rect.x = static_cast<int>(screenX);
	rect.y = static_cast<int>(screenY);
	rect.w = static_cast<int>(cell_size * cam_.zoom);
	rect.h = static_cast<int>(cell_size * cam_.zoom);

	return rect;
}

void Game::drawGrid() {
	constexpr uint8_t default_grid_color = 200;
	constexpr uint8_t default_grid_alpha = 255;

	auto const left = cam_.x - screen_width_half / cam_.zoom;
	auto const right = cam_.x + screen_width_half / cam_.zoom;
	auto const top = cam_.y - screen_height_half / cam_.zoom;
	auto const bottom = cam_.y + screen_height_half / cam_.zoom;

	int const leftCell = static_cast<int>(floor(left / cell_size));
	int const rightCell = static_cast<int>(ceil(right / cell_size));
	int const topCell = static_cast<int>(floor(top / cell_size));
	int const bottomCell = static_cast<int>(ceil(bottom / cell_size));

	SDL_SetRenderDrawColor(renderer_, default_grid_color, default_grid_color, default_grid_color, default_grid_alpha);

	for (int itx = leftCell; itx <= rightCell; ++itx) {
		for (int ity = topCell; ity <= bottomCell; ++ity) {
			SDL_Rect const rect = worldToScreen(static_cast<float>(itx) * cell_size, static_cast<float>(ity) * cell_size);
			SDL_RenderDrawRect(renderer_, &rect);
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
	const uint8_t defaultBorderColor = 255;
	const auto zoomFactorIncrease = 1.1F;
	const auto zoomFactorDecrease = 0.9F;
	const auto delay = 16;

	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) running = false;

			if (event.type == SDL_MOUSEWHEEL) {
				int mouseX{};
				int mouseY{};
				SDL_GetMouseState(&mouseX, &mouseY);

				float const beforeZoomX = cam_.x + static_cast<float>(mouseX - screen_width_half) / cam_.zoom;
				float const beforeZoomY = cam_.y + static_cast<float>(mouseY - screen_height_half) / cam_.zoom;
				float const afterZoomX = cam_.x + static_cast<float>(mouseX - screen_width_half) / cam_.zoom;
				float const afterZoomY = cam_.y + static_cast<float>(mouseY - screen_height_half) / cam_.zoom;

				cam_.zoom *= (event.wheel.y > 0) ? zoomFactorIncrease : zoomFactorDecrease;
				cam_.x += (beforeZoomX - afterZoomX);
				cam_.y += (beforeZoomY - afterZoomY);
			}

			if (event.type == SDL_MOUSEMOTION && ((event.motion.state & SDL_BUTTON_LMASK) != 0U)) {
				cam_.x -= static_cast<float>(event.motion.xrel) / cam_.zoom;
				cam_.y -= static_cast<float>(event.motion.yrel) / cam_.zoom;
			}
		}

		SDL_SetRenderDrawColor(renderer_, defaultBorderColor, defaultBorderColor, defaultBorderColor,
							   defaultBorderColor);
		SDL_RenderClear(renderer_);

		drawGrid();

		SDL_RenderPresent(renderer_);
		SDL_Delay(delay);
	}
}