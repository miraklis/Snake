#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdHeaders.h"
#include "Object.h"

namespace Snake {

	class GameObject : public Object {
	public:
		void Show();
		void Hide();
		bool IsVisible();
		bool IsActive();
		void Activate();
		void Deactivate();
	protected:
		GameObject(int x, int y, bool visible, bool active);
		GameObject(int x, int y, size_t w, size_t h, bool visible, bool active);
		virtual ~GameObject();
		virtual void Render() = 0;
		void LoadTexture(string texture);
		SDL_Texture* texture;
		bool visible;
		bool active;
	};

}

#endif // GAMEOBJECT_H