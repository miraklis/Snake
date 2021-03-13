#include "Object.h"

namespace Snake {

	Object::Object() 
			:Object(0,0,0,0) {}

	Object::Object(int x, int y)
			:Object(x,y,0,0) {}

	Object::Object(int x, int y, size_t w, size_t h) {
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
	}

	Object::~Object() {}

	const SDL_Rect& Object::GetRect() {
		return rect;
	}

	void Object::SetRect(const SDL_Rect& rect) {
		this->rect = rect;
	}
}