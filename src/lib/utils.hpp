#pragma once

#include <cinttypes>

struct Camera {
	float x = 0.0F;
	float y = 0.0F;
	float zoom = 1.0F;
};

struct GameInfo {
	uint32_t speed;
};

GameInfo parseOptions(int argc, char **argv);
