#include "FruitManager.h"
#include "Fruit.h"
#include "Game.h"
#include "Utils.h"

namespace Snake {

	FruitManager::FruitManager() {
		texture = Snake::LoadTexture("assets/fruit.png");
		totalFruits = 0 ;
	}

	FruitManager::~FruitManager() {
		SDL_DestroyTexture(texture);
	}
	
	void FruitManager::GenerateFruits(/*unique_ptr<Board>& board, */int numOfFruits) {
		thread t([=/*, &board*/]() mutable {
			unique_lock<mutex> lock(mux);
			while(numOfFruits-- > 0) {
				vector<pair<int, int>> fruitPos = Game::Gameboard->GetRegionEmpty();
				SDL_Rect fruitRect = Game::Gameboard->GetCellRect(fruitPos[0].first, fruitPos[0].second);
				unique_ptr<Fruit> fruit = MakeRandomFruit(fruitPos, fruitRect);
				for(auto& p : fruit->GetBoardCoords()) {
					Game::Gameboard->SetCell(p.first, p.second, CHAR_FRUIT); // (char)Board::CellKind::Fruit);
				}
				fruits.push_back(move(fruit));
				totalFruits++;
			}
		});
		t.detach();
	}

	unique_ptr<Fruit> FruitManager::MakeRandomFruit(vector<pair<int, int>>& fruitPos, SDL_Rect& rect) {
		unsigned int seed = static_cast<unsigned int>(time(nullptr) + totalFruits);
		srand(seed);
		uint8_t f = rand() % (uint8_t)Fruit::FruitKind::Last;
		return make_unique<Fruit>(totalFruits, fruitPos, rect, (Fruit::FruitKind)f);
	}

	void FruitManager::RenderAll() {
		for(auto& fruit : fruits) {
			fruit->Render(texture);
		}
	}

	void FruitManager::ClearFruits() {
		fruits.clear();
		totalFruits = 0;
	}

	Fruit& FruitManager::GetFruitAt(int x, int y) {
		for(auto& f : fruits) {
			for(auto& p : f->GetBoardCoords()) {
				if(p.first == x && p.second == y) {
					return *f;
				}
			}
		}
		return *fruits[0];
	}

	void FruitManager::RemoveFruit(Fruit& fruit) {
		fruits.erase(remove_if(fruits.begin(), fruits.end(), [&](auto const& f) { return *f == fruit; }), fruits.end());
	}

	pair<int, int> FruitManager::GetFruitCoords(int fruitID)
	{
		return fruits[fruitID]->GetBoardCoords()[0];
	}

}