#include "Fruit.h"
#include "Game.h"

namespace Snake {

	Fruit::Fruit(int id, vector<pair<int, int>>& fruitPos, SDL_Rect& fruitRect, FruitKind kind) : GameObject(fruitRect.x, fruitRect.y, true, true) {
		this->id = id;
		this->kind = kind;
		switch(kind) {
			case FruitKind::Apple:
				growBy = 2;
				break;
			case FruitKind::Pear:
				growBy = 5;
				break;
			case FruitKind::Orange:
				growBy = 10;
				break;
			case FruitKind::Apricot:
				growBy = 12;
				break;
			case FruitKind::Pomegranate:
				growBy = 15;
				break;
			case FruitKind::Peach:
				growBy = 20;
				break;
			default:
				growBy = 0;
				break;
		}
		position.w = fruitRect.w * 2;
		position.h = fruitRect.h * 2;
		this->boardCoords = fruitPos;
		src.x = (uint8_t)kind * 122;
		src.y = 0; 
		src.w = 122;
		src.h = 115;
		visible = true;
	}

	Fruit::~Fruit() {
	}

	bool Fruit::operator==(const Fruit& other) {
		return this->id == other.id;
	}

	size_t Fruit::GrowBy() {
		return growBy;
	}

	void Fruit::Render(SDL_Texture* texture) {
		if(!visible)
			return;
		SDL_RenderCopy(Game::Renderer, texture, &src, &position);
	}

	vector<pair<int, int>>& Fruit::GetBoardCoords() {
		return boardCoords;
	}

}