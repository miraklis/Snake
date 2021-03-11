#pragma once

#include "stdHeaders.h"
#include "SDL.h"
#include "GameObject.h"

namespace Snake {

	class Fruit : public GameObject {
	public:
		enum class FruitKind : uint8_t { Apple = 0, Pear, Orange, Apricot, Pomegranate, Peach, Last };
		Fruit(int id, vector<pair<int, int>>& fruitPos, SDL_Rect& fruitRect, FruitKind kind);
		~Fruit();
		bool operator== (const Fruit& other);
		size_t GrowBy();
		void Render() override {};
		void Render(SDL_Texture* texture);
		vector<pair<int, int>>& GetBoardCoords();
	private:
		int id;
		FruitKind kind;
		size_t growBy;
		SDL_Rect src;
		vector<pair<int, int>> boardCoords;
	};

}

