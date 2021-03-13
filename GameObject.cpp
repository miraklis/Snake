#include "GameObject.h"
#include "Utils.h"

namespace Snake {

	GameObject::GameObject(int x, int y, bool visible, bool active) 
		:GameObject(x, y, 0, 0, visible, active) {
	}

	GameObject::GameObject(int x, int y, size_t w, size_t h, bool visible, bool active)
			:Object(x,y,w,h) {
		texture = nullptr;
		this->visible = visible;
		this->active = active;
	}

	GameObject::~GameObject() {}

	void GameObject::LoadTexture(string texture) {
		this->texture = Snake::LoadTexture(texture);
	}

	bool GameObject::IsActive() {
		return active;
	}

	void GameObject::Activate() {
		active = true;
	}

	void GameObject::Deactivate() {
		active = false;
	}

	void GameObject::Show() {
		visible = true;
	}

	void GameObject::Hide() {
		visible = false;
	}

	bool GameObject::IsVisible() {
		return visible;
	}

}