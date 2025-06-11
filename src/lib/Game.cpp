#include "Game.hpp"
#include "utils.hpp"
#include <SDL.h>
#include <array>
#include <cmath>
#include <unordered_map>

SDL_FRect Game::worldToScreenCell(float worldX, float worldY) const {
	auto const screenX = (worldX - cam_.x) * cam_.zoom + screenWidthHalf_;
	auto const screenY = (worldY - cam_.y) * cam_.zoom + screenHeightHalf_;

	return {screenX, screenY, cell_size * cam_.zoom, cell_size * cam_.zoom};
}

void Game::drawGrid() {
	constexpr Uint8 default_border_color = 128;
	constexpr Uint8 default_alive_color = 255;

	auto const left = cam_.x - screenWidthHalf_ / cam_.zoom;
	auto const right = cam_.x + screenWidthHalf_ / cam_.zoom;
	auto const top = cam_.y - screenHeightHalf_ / cam_.zoom;
	auto const bottom = cam_.y + screenHeightHalf_ / cam_.zoom;

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

void Game::step() {
	std::array<int, 3> const delta = {-1, 1, 0};
	std::unordered_map<std::pair<int, int>, int, PairHash> neighborCount;


	for (const auto &alive: alive_cells_) {
		auto const &[aliveX, aliveY] = alive;
		for (auto deltaX: delta) {
			for (auto deltaY: delta) {
				if (deltaX == deltaY && deltaX == 0) continue;
				neighborCount[{aliveX + deltaX, aliveY + deltaY}]++;
			}
		}
	}


	std::unordered_set<std::pair<int, int>, PairHash> newAlive;

	for (auto &[cell, count]: neighborCount) {
		if (count == 3 || (count == 2 && alive_cells_.contains(cell))) { newAlive.insert(cell); }
	}

	alive_cells_ = std::move(newAlive);

}

Game::Game(const GameInfo &info)
	: info_(info), cam_(Camera()),
	  window_(SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth_,
							   screenHeight_, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)),
	  renderer_(SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED)) {}

Game::~Game() {
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

void Game::run() {
	constexpr Uint8 default_background_color = 0;
	constexpr Uint32 render_delay = 16;
	const auto zoomFactorIncrease = 1.1F;
	const auto zoomFactorDecrease = 0.9F;

	bool running = true;
	bool pause = true;
	SDL_Event event;
	Uint32 lastLogicTick = SDL_GetTicks();

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
						auto worldX = static_cast<float>(event.button.x - screenWidthHalf_) / cam_.zoom + cam_.x;
						auto worldY = static_cast<float>(event.button.y - screenHeightHalf_) / cam_.zoom + cam_.y;
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
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_SPACE) { pause = !pause; }
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
						screenWidth_ = event.window.data1;
						screenHeight_ = event.window.data2;
						screenWidthHalf_ = screenWidth_ / 2;
						screenHeightHalf_ = screenHeight_ / 2;
					}
					break;
				default:
					break;
			}
		}

		Uint32 const currentTick = SDL_GetTicks();
		if (!pause && currentTick - lastLogicTick >= info_.speed) {
			step();
			lastLogicTick = currentTick;
		}

		SDL_SetRenderDrawColor(renderer_, default_background_color, default_background_color, default_background_color,
							   default_alpha);
		SDL_RenderClear(renderer_);

		drawGrid();

		SDL_RenderPresent(renderer_);
		SDL_Delay(render_delay);
	}
}
