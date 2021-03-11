#include "Object.h"

namespace Snake {

	Object::Object() 
		:Object(0,0) {}

	Object::Object(int x, int y) {
		position.x = x;
		position.y = y;
		position.w = position.h = 0;
	}

	Object::Object(int x, int y, size_t w, size_t h)
		:Object(x, y)
	{
		position.w = w;
		position.h = h;
	}


	Object::~Object() {}

	const SDL_Rect& Object::GetRect() {
		return position;
	}

	void Object::SetRect(const SDL_Rect& rect) {
		position = rect;
	}
}