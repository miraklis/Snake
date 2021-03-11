#include "AI.h"
#include "Game.h"

namespace Snake
{

	Snake::AI::AI()
	{
		// Get board dimensions to adjust nodes array
		gridWidth = Game::Gameboard->GetBoardWidth();
		gridHeight = Game::Gameboard->GetBoardHeight();
		nodes = new Node[gridWidth * gridHeight];
		for(int x = 0; x < gridWidth; x++) {
			for(int y = 0; y < gridHeight; y++) {
				if(y > 0)
					nodes[y * gridWidth + x].neighbours.push_back(&nodes[(y - 1) * gridWidth + x]);
				if(y < gridHeight - 1)
					nodes[y * gridWidth + x].neighbours.push_back(&nodes[(y + 1) * gridWidth + x]);
				if(x > 0)
					nodes[y * gridWidth + x].neighbours.push_back(&nodes[y * gridWidth + (x - 1)]);
				if(x < gridWidth - 1)
					nodes[y * gridWidth + x].neighbours.push_back(&nodes[y * gridWidth + (x + 1)]);
			}
		}
		InitNodes();
		goal = nullptr;
	}

	Snake::AI::~AI()
	{
		delete[] nodes;
	}

	bool AI::HasPath() const
	{
		return path.size() > 0;
	}

	bool Snake::AI::FindShortestPath(pair<int, int> startPoint)
	{
		InitNodes();
		ClearPath();

		auto distance = [](Node* a, Node* b)
		{
			return sqrtf(static_cast<float>((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y)));
		};

		auto heuristic = [distance](Node* a, Node* b)
		{
			return distance(a, b);
		};

		Node* start = &nodes[startPoint.second * gridWidth + startPoint.first];
		Node* end = FindClosestTarget(start);
		goal = end;
		start->localGoal = 0.0f;
		start->globalGoal = heuristic(start, end);
		Node* current = start;
		list<Node*> notTestedNodes;
		notTestedNodes.clear();
		notTestedNodes.push_back(start);
		while(!notTestedNodes.empty() && current != end)
		{
			notTestedNodes.sort([](const Node* lhs, const Node* rhs)
			{
				return lhs->globalGoal < rhs->globalGoal;
			});
			while(!notTestedNodes.empty() && notTestedNodes.front()->visited)
				notTestedNodes.pop_front();
			if(notTestedNodes.empty())
				break;
			current = notTestedNodes.front();
			current->visited = true; // We only explore a node once
			for(auto nodeNeighbour : current->neighbours)
			{
				if(!nodeNeighbour->visited && !nodeNeighbour->obstacle)
					notTestedNodes.push_back(nodeNeighbour);
				float fPossiblyLowerGoal = current->localGoal + distance(current, nodeNeighbour);
				if(fPossiblyLowerGoal < nodeNeighbour->localGoal)
				{
					nodeNeighbour->parent = current;
					nodeNeighbour->localGoal = fPossiblyLowerGoal;
					nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, end);
				}
			}
		}
		if(end != nullptr) {
			Node* p = end;;
#ifdef DEBUG
			nodesToShow.clear();
#endif
			while(p->parent != nullptr)
			{
#ifdef DEBUG
				nodesToShow.push_back(p);
#endif
				if(p->parent->x < p->x)
					path.push(0); // go right
				if(p->parent->x > p->x)
					path.push(2); // go left
				if(p->parent->y < p->y)
					path.push(1); // go down
				if(p->parent->y > p->y)
					path.push(3); // go up
				p = p->parent;
			}
			return true;
		} else {
			return false;
		}
	}

	AI::Node* AI::FindClosestTarget(Node* startPoint)
	{
		list<Node*> notTested;
		notTested.push_back(startPoint);

		Node* current = startPoint;
		float maxDistance = 0.0f;

		while(!notTested.empty() && Game::Gameboard->GetCell(current->x, current->y) != CHAR_FRUIT)
		{
			while(!notTested.empty() && notTested.front()->checked)
				notTested.pop_front();

			if(notTested.empty())
				break;

			current = notTested.front();
			current->checked = true;

			for(auto n : current->neighbours) {
				if(!n->obstacle)
					notTested.push_back(n);
			}
		}
		return current;
	}

	int8_t AI::GetNextMove()
	{
		int8_t i{ -1 };
		if(path.size() > 0) {
			i = path.top();
			path.pop();
		}
		return i;
	}

	void AI::ClearPath()
	{
		while(!path.empty())
			path.pop();
	}

	AI::Node* AI::GetGoal() const
	{
		return goal;
	}

	void AI::InitNodes()
	{
		for(int x = 0; x < gridWidth; x++) {
			for(int y = 0; y < gridHeight; y++)
			{
				nodes[y * gridWidth + x].x = x;
				nodes[y * gridWidth + x].y = y;
				if(Game::Gameboard->GetCell(x, y) == CHAR_EMPTY || Game::Gameboard->GetCell(x, y) == CHAR_FRUIT)
					nodes[y * gridWidth + x].obstacle = false;
				else
					nodes[y * gridWidth + x].obstacle = true;
				nodes[y * gridWidth + x].parent = nullptr;
				nodes[y * gridWidth + x].visited = false;
				nodes[y * gridWidth + x].checked = false;
				nodes[y * gridWidth + x].globalGoal = INFINITY;
				nodes[y * gridWidth + x].localGoal = INFINITY;
			}
		}
	}
}