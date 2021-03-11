#pragma once

//#define DEBUG

#include "SDL.h"

namespace Snake {

	// System Colors
	const SDL_Color TRANSPARENT_COLOR{ 0, 0, 0, 0 };
	const SDL_Color WHITE_COLOR{ 255, 255, 255, 255 };
	const SDL_Color BLACK_COLOR{ 0, 0, 0, 255 };
	const SDL_Color RED_COLOR{ 255, 0, 0, 255 };
	const SDL_Color GREEN_COLOR{ 0, 255, 0, 255 };
	const SDL_Color BLUE_COLOR{ 0, 0, 255, 255 };
	const SDL_Color YELLOW_COLOR{ 255, 255, 0, 255 };
	const SDL_Color DARK_RED_COLOR{ 32, 0, 0, 255 };
	const SDL_Color DARK_GREEN_COLOR{ 0, 32, 0, 255 };
	const SDL_Color DARK_BLUE_COLOR{ 0, 0, 32, 255 };

	// Game Constants
	const size_t INITIAL_FRUITS{ 3 };
	const char CHAR_EMPTY{ ' ' };
	const char CHAR_WALL{ '#' };
	const char CHAR_FRUIT{ 'F' };
	const char CHAR_HEAD{ 'H' };
	const char CHAR_BODY{ 'B' };
	const char CHAR_CURVE{ 'C' };
	const char CHAR_TAIL{ 'T' };

	// Player Constants
	const size_t INITIAL_LIVES{ 3 };
	const size_t INITIAL_SPEED{ 20 };
	const int MAX_BODY{ 20 };
	const int MAX_SPEED{ 60 };
}