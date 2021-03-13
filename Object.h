#ifndef OBJECT_H
#define OBJECT_H

#include "SDL.h"

namespace Snake {

	class Object {
	public:
		~Object();
		const SDL_Rect& GetRect();
		void SetRect(const SDL_Rect& rect);
	protected:
		Object();
		Object(int x, int y);
		Object(int x, int y, size_t w, size_t h);
		SDL_Rect rect;
	};

}

#endif //OBJECT_H