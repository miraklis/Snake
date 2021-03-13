#ifndef AI_H
#define AI_H

#include "stdHeaders.h"

namespace Snake
{
	class AI
	{
	public:
		struct Node
		{
			bool obstacle;
			bool visited;
			bool checked;
			int x;
			int y;
			float globalGoal;
			float localGoal;
			vector<Node*> neighbours;
			Node* parent;
		};
	private:
		Node* nodes;
		Node* goal;
		stack<int8_t> path;
		int gridWidth;
		int gridHeight;
		void InitNodes();
	public:
		AI();
		~AI();
		bool HasPath() const;
		bool FindShortestPath(pair<int, int> startPoint);
		Node* FindClosestTarget(Node* startPoint);
		int8_t GetNextMove();
		void ClearPath();
		Node* GetGoal() const;
#ifdef DEBUG
		vector<Node*> nodesToShow;
#endif
	};

}

#endif // AI_H