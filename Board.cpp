#include "Board.h"
#include "Game.h"
#include "FruitManager.h"
#include "Utils.h"

namespace Snake {

    const int BOARD_WIDTH {80};
    const int BOARD_HEIGHT {50};

    Board::Board(int x, int y, size_t w, size_t h) 
            :GameObject(x,y,w,h,false, false) {
        texture = Snake::LoadTexture("assets/brick.png");
        // Normalize size
        cellWidth = rect.w / BOARD_WIDTH;
        cellHeight = rect.h / BOARD_HEIGHT;
        rect.w = cellWidth * BOARD_WIDTH;
        rect.h = cellHeight * BOARD_HEIGHT;
        matrix = new char[BOARD_WIDTH * BOARD_HEIGHT];
        ClearMatrix();
    }

    Board::~Board() {
        delete[] matrix;
        SDL_DestroyTexture(texture);
    }

    void Board::Render() {
        if(!visible)
            return;
        // Render walls
        SDL_Rect wallSrc {23, 11, 45, 23};
        SDL_Rect r{ rect.x, rect.y, cellWidth, cellHeight };
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            for(int i = 0; i < BOARD_WIDTH; i++) {
                r.x = rect.x + (i * cellWidth);
                r.y = rect.y + (j * cellHeight);
                char c = GetCell(i, j);
                if (c == CHAR_WALL) {
                    SDL_RenderCopy(Game::Renderer, texture, &wallSrc, &r);
                }
            }
        }
    }

    SDL_Rect Board::GetCellRect(int x, int y) {
        if(!CheckLimits(x, y))
            return {0,0,0,0};
        SDL_Rect r;
        r.w = cellWidth;
        r.h = cellHeight;
        r.x = rect.x + (x * cellWidth);
        r.y = rect.y + (y * cellHeight);

        return r;
    }

	void Board::ClearRange(vector<pair<int, int>> coords) {
        for(auto& c : coords) {
            SetCell(c.first, c.second, CHAR_EMPTY);
        }
    }

    char Board::GetCell(int x, int y) {  
        if(!CheckLimits(x, y))
            return 0;
        return matrix[y*BOARD_WIDTH+x];
    }

    void Board::SetCell(int x, int y, char c) {
        if(!CheckLimits(x, y))
            return;
        matrix[y*BOARD_WIDTH+x] = c;
    }

    void Board::ClearMatrix() {
        for(int i = 0; i < BOARD_WIDTH; i++) {
            for(int j = 0; j < BOARD_HEIGHT; j++) {
                SetCell(i, j, CHAR_EMPTY);
            }
        }
        FruitManager::GetInstance().ClearFruits();
    }

    char* Board::GetRandomEmpty(int& outX, int& outY) {
        int x, y;
        static unsigned int attempt{ 0 };
        unsigned int seed = static_cast<unsigned int>(time(nullptr) + attempt);
        srand(seed);
        do {
            x = 1 + (rand() % (BOARD_WIDTH - 2));
            y = 1 + (rand() % (BOARD_HEIGHT - 2));
        } while(GetCell(x, y) != CHAR_EMPTY);
        attempt++;
        outX = x;
        outY = y;
        return &matrix[y*BOARD_WIDTH+x];
    }

    vector<pair<int, int>> Board::GetRegionEmpty(size_t rows, size_t cols) {
        vector<pair<int, int>> region;
        bool found = false;
        int x, y;
        while(!found) {
            GetRandomEmpty(x, y);
            found = isRangeEmpty(x, y, rows, cols, region);
        }
        return move(region);
    }

    bool Board::CheckLimits(int x, int y) {
        return (x >= 0 && y>=0 && x < BOARD_WIDTH && y < BOARD_HEIGHT);
    }

    int Board::GetBoardWidth() const
    {
        return BOARD_WIDTH;
    }

    int Board::GetBoardHeight() const
    {
        return BOARD_HEIGHT;
    }

    bool Board::isRangeEmpty(int x, int y, size_t rows, size_t cols, vector<pair<int, int>>& outRange) {
        bool isEmpty = true;
        outRange.clear();
        for(int r = y, rend = r + rows; isEmpty && r < rend; r++) {
            for(int c = x, cend = c + cols; isEmpty && c < cend; c++) {
                if(GetCell(c, r) != CHAR_EMPTY) {
                    isEmpty = false;
                } else {
                    outRange.push_back({ c, r });
                }
            }
        }
        return isEmpty;
    }

 }