#pragma once

#include "stdHeaders.h"
#include "SDL.h"
#include "GameObject.h"
#include "Event.h"
#include "Timer.h"
#include "Board.h"
#include "Fruit.h"
#include "UIPanel.h"
#include "AI.h"

namespace Snake {

    class Player : public GameObject, public enable_shared_from_this<Player> {
        public:
            enum class PlayerStatus { Idle = 0, Starting, Paused, Playing, Crashed, Dead };
            enum class Direction :int8_t { Right = 0, Down, Left, Up };
            enum class InitLevel { NewPlayer, NewLevel, Respawn };
            bool operator== (const Player& other);
            Player(string imgFile, int id, bool human = true);
            ~Player() override;
            void Init(InitLevel initLevel);
			void EraseFromBoard();
            void PutOnBoard(int x, int y);
            void Turn(Direction direction);
            void Grow(int growBy);
            void SpeedUp(int speedUpBy);
            void Update();
            void Render();
            void PauseStartTimer();
            void CreateInfoPanel(int x, int y, size_t w, size_t h, SDL_Color bgColor, SDL_Color fgColor,
                                 SDL_GUI::HorizontalAlign halign, SDL_GUI::VerticalAlign valign);
            int GetId() const;
            string Who() const;
            int GetSpeed() const;
            int GetScore() const;
            int GetSize() const;
            int GetLevelScore() const;
            int GetWins() const;
            void MakeAWin();
            uint8_t GetLives() const;
            bool IsStarting() const;
            bool IsPlaying() const;
            bool IsCrashed() const;
            bool IsDead() const;
            bool IsHuman() const;
            void CrashedEvent();
            Event<shared_ptr<Player>> OnCrashedEvent;
            void Activate();
            void ActivatedEvent();
            void Deactivate();
            void Pause();
            void UnPause();
            int TimeToStart() const;
            void FruitEatenEvent(Fruit& fruit);
            Event<shared_ptr<Player>, Fruit&> OnFruitEatenEvent;
        private:
            struct SnakePart {
                SDL_Rect rect;
                Direction direction;
                char kind;
                int boardCellX;
                int boardCellY;
                bool clockwise;
            };
            vector<SnakePart> sBody;
            unique_ptr<AI> ai;
            uint8_t lives;
            unique_ptr<Timer> startTimer;
            PlayerStatus status;
            PlayerStatus prevStatus;
            bool wasActive;
            bool turning;
            bool clockwise;
            int id;
            int bodySize;
            int growBy;
            int delay;
            int speed;
            int score;
            int levelScore;
            int wins;
            bool human;
            void updateInfoPanel();
            bool checkCollisions(int x, int y);
            bool canChangeDirection(Direction newDir);
            char getNextCellForHead(Direction newDir) const;

            unique_ptr<UIPanel> infoPanel;
    };
}