#pragma once

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
		virtual ~GameObject();
		virtual void Render() = 0;
		void LoadTexture(string texture);
		SDL_Texture* texture;
		bool visible;
		bool active;
	};

}