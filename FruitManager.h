#pragma once

#include "stdHeaders.h"
#include "SDL.h"
#include "Singleton.h"
#include "Fruit.h"
//#include "Board.h"

namespace Snake {
	class FruitManager : public Singleton<FruitManager> {
	private:
		FruitManager();
		friend class Singleton<FruitManager>;
		SDL_Texture* texture;
		int totalFruits;
		vector<unique_ptr<Fruit>> fruits;
		mutex mux;
	public:
		~FruitManager();
		void GenerateFruits(/*unique_ptr<Board>& board, */int numOfFruits);
		unique_ptr<Fruit> MakeRandomFruit(vector<pair<int, int>>& fruitPos, SDL_Rect& rect);
		void RenderAll();
		void ClearFruits();
		Fruit& GetFruitAt(int x, int y);
		void RemoveFruit(Fruit& fruit);
		pair<int, int> GetFruitCoords(int fruitID);
	};

}