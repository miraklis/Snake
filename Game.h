#ifndef GAME_H
#define GAME_H

#include "stdHeaders.h"
#include "SDL.h"
#include "Board.h"
#include "Player.h"
#include "Fruit.h"

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
        bool Running() const;
        GameType GetType() const;
        GameStatus GetStatus() const;
        pair<size_t, size_t> GetScreenSize() const;
        static SDL_Renderer* Renderer;
        static unique_ptr<Board> Gameboard;
        // Event Handlers
        void playerCrashed(shared_ptr<Player> player);
        void playerAteFruit(shared_ptr<Player> player, Fruit& fruit);
    private:
        SDL_Window* window;
        shared_ptr<SDL_GUI::UIMenu> menu;
        unique_ptr<SDL_GUI::UIPanel> banner;
        unique_ptr<UIImage> splashScreen;
        GameType gameType;
        GameStatus gameStatus;
        bool isRunning;
        int screenWidth;
        int screenHeight;
        void clean();
        bool menuCommand(string cmd);
        void startNewGame(string type);
        void startNewLevel();
    };
}

#endif GAME_H