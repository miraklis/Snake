#ifndef BOARD_H
#define BOARD_H

#include "stdHeaders.h"
#include "SDL.h"
#include "GameObject.h"
#include "AI.h"

#include "SDL_GUI.h"
using namespace SDL_GUI;

namespace Snake {
    class Board : public GameObject {
        public:
            Board(int x, int y, size_t w, size_t h);
            ~Board() override;
            void Render() override;
            void ClearMatrix();
            char GetCell(int x, int y);
            void SetCell(int x, int y, char c);
            char* GetRandomEmpty(int& outX, int& outY);
            vector<pair<int, int>> GetRegionEmpty(size_t rows=2, size_t cols=2);
            SDL_Rect GetCellRect(int x, int y);
            void ClearRange(vector<pair<int, int>> coords);
            bool CheckLimits(int x, int y);
            int GetBoardWidth() const;
            int GetBoardHeight() const;
        private:
            int cellWidth, cellHeight;
            char* matrix;
            bool isRangeEmpty(int x, int y, size_t rows, size_t cols, vector<pair<int, int>>& outRange);
    };
}

#endif // BOARD_H