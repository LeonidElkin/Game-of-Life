#pragma once

#include <cinttypes>

struct Camera {
	float x = 0.0F;
	float y = 0.0F;
	float zoom = 1.0F;
};

enum class GameType : uint8_t {
	BorderedGameType,
	ClosureGameType,
};

struct GameInfo {
	uint32_t width;
	uint32_t height;
	GameType type;
};

GameInfo parseOptions(int argc, char **argv);
