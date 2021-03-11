#pragma once

#include "stdHeaders.h"
#include "SDL.h"
//#include "UIImage.h"
//#include "UIPanel.h"
//#include "UIMenu.h"
#include "Board.h"
#include "Player.h"
#include "Fruit.h"
//#include "Net.h"

#include "SDL_GUI.h"

namespace Snake {

    class Game {
    public:
        Game();
        ~Game();
        enum class GameType {OnePlayer, TwoPlayers, PvC, LanHostGame, LanJoinGame};
        enum class GameStatus { SplashScreen, Playing, LevelCompleted, GameOver, GameFinished };
        void Init();
        void HandleEvents();
        void Update();
        void Render();
        bool Running();
        GameType GetType();
        GameStatus GetStatus();
        size_t NumOfPlayers() const;
        static SDL_Renderer* Renderer;
        static unique_ptr<Board> Gameboard;
    private:
        SDL_Window* window;
        shared_ptr<SDL_GUI::UIMenu> menu;
        unique_ptr<SDL_GUI::UIPanel> banner;
        unique_ptr<UIImage> splashScreen;
        //Net* net;
        GameType gameType;
        GameStatus gameStatus;
        bool isRunning;
        int screenWidth;
        int screenHeight;
        size_t totalPlayers;
        void clean();
        bool menuCommand(string cmd);
        void startNewGame(string type);
        void startNewLevel();
        // Event Handlers
        void playerCrashed(shared_ptr<Player> player);
        void playerAteFruit(shared_ptr<Player> player, Fruit& fruit);
    };
}