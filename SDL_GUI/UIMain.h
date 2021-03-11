#ifndef UIMAIN_H
#define UIMAIN_H

// STL dependencies
#include <iostream>
#include <string>
#include <functional>
//using namespace std;

// External dependencies
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Event.h"

namespace SDL_GUI {

	struct sFonts {
		static const char* TTF_TIMES;
		static const char* TTF_ARCADE;
		static const char* TTF_BLOX2;
		static const char* TTF_KA1;
	};

	enum class HorizontalAlign {
		Left, Center, Right
	};

	enum class VerticalAlign {
		Top, Middle, Bottom
	};

	extern SDL_Texture* LoadTexture(SDL_Renderer* renderer, std::string file);

}

#endif //UIMAIN_H
