#include "Game.h"
#include "Utils.h"
#include "FruitManager.h"
#include "PlayerManager.h"
#include "LevelManager.h"
#include "Timer.h"

#include "SDL_GUI.h"

namespace Snake {
    
    const SDL_Color BACKGROUND_COLOR{ DARK_GREEN_COLOR };

    SDL_Renderer* Game::Renderer = nullptr;
    unique_ptr<Board> Game::Gameboard = nullptr;

    Game::Game() {
        gameStatus = GameStatus::SplashScreen;
        isRunning = false;
        window = nullptr;
        menu = nullptr;
        banner = nullptr;
        splashScreen = nullptr;
    }

    Game::~Game() {
        clean();
    }

    void Game::Init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            ErrorMsg("SDL INITIALIZATION", SDL_GetError());
            clean();
            exit(1);
        }
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        screenWidth = dm.w;
        screenHeight = dm.h;

        window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        if (!window) {
            ErrorMsg("WINDOW CREATION", SDL_GetError());
            clean();
            exit(1);
        }
        Renderer = SDL_CreateRenderer(window, -1, 0);
        if (!Renderer) {
            ErrorMsg("RENDERER CREATION", SDL_GetError());
            clean();
            exit(1);
        }
        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
        if (TTF_Init() < 0) {
            ErrorMsg("TTF INIT", TTF_GetError());
            clean();
            exit(1);
        }
        size_t boardWidth = static_cast<size_t>(screenWidth * 0.8); // 80% of screen's width
        Gameboard = make_unique<Board>((screenWidth / 2) - (boardWidth / 2), 0, boardWidth, screenHeight);
        SDL_Rect boardRect = Gameboard->GetRect();
        int midBoardX = boardRect.x + (boardRect.w / 2);
        int midBoardY = boardRect.y + (boardRect.h / 2);
        std::string fnt = "assets/";
        fnt.append(SDL_GUI::sFonts::TTF_ARCADE);
        banner = make_unique<SDL_GUI::UIPanel>(Renderer, "banner", 0, 0, 0, 0, true, true,
                                               fnt, 72,
                                               SDL_GUI::HorizontalAlign::Center, SDL_GUI::VerticalAlign::Middle, SDL_GUI::VerticalAlign::Middle, 
                                               TRANSPARENT_COLOR, WHITE_COLOR);
        banner->Hide();
        splashScreen = make_unique<SDL_GUI::UIImage>(Renderer, "assets/splash-screen.png", 0, 0);
        splashScreen->SetPos((screenWidth / 2) - (splashScreen->GetRect().w / 2), (screenHeight / 2) - (splashScreen->GetRect().h / 2));
        splashScreen->Show();
        menu = make_shared<SDL_GUI::UIMenu>(Renderer, "menu", midBoardX, midBoardY, 0, 0,
                                            fnt, 48,
                                            SDL_GUI::HorizontalAlign::Center, SDL_GUI::VerticalAlign::Middle, SDL_GUI::VerticalAlign::Middle,
                                            DARK_BLUE_COLOR, WHITE_COLOR, BLUE_COLOR, WHITE_COLOR,
                                            nullptr, bind(&Game::menuCommand, this, placeholders::_1));
        menu->AddItem("oneplayer", "One Player Game");
        menu->AddItem("twoplayers", "Two Player Game");
        menu->AddItem("pvc", "Player vs Computer");
        menu->AddItem("exit", "Exit Game");
        menu->SetPos((screenWidth / 2) - (menu->GetRect().w / 2), (screenHeight / 2) - (menu->GetRect().h / 2));
        menu->Show();
        isRunning = true;
    }

    void Game::HandleEvents() {
        if(menu->IsVisible()) {
            menu->HandleEvents();
            return;
        }
        SDL_Event event;
        SDL_PollEvent(&event);
        bool keyHandled = false;
        switch(event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_SPACE: // Change Level or Respawn player
                        if(gameStatus == GameStatus::LevelCompleted) {
                            startNewLevel();
                            keyHandled = true;
                            break;
                        }
                        break;
                    case SDLK_ESCAPE:
                        PlayerManager::GetInstance().PausePlayers();
                        menu->Show();
                        keyHandled = true;
                        break;
                    default:
                        break;
                }
                if(!keyHandled && gameStatus == GameStatus::Playing)
                    PlayerManager::GetInstance().HandlePlayersInput(event.key.keysym.sym);
                break;
            default:
                break;
        }
    }

    bool Game::menuCommand(string cmd) {
        if(cmd == "esc") {
            if(menu->GetParent() != nullptr) {
                menu = menu->GetParent();
                return false;
            }
            if(gameStatus == GameStatus::SplashScreen)
                return false;
            PlayerManager::GetInstance().UnPausePlayers();
            menu->Hide();
            return true;
        }
        if(cmd == "exit") {
            isRunning = false;
            return true;
        }
        menu->Hide();
        startNewGame(cmd);
        return true;
    }

    void Game::startNewLevel() {
        banner->Hide();
        gameStatus = GameStatus::Playing;
        LevelManager::GetInstance().LoadNextLevel();
        FruitManager::GetInstance().GenerateFruits(INITIAL_FRUITS);
        PlayerManager::GetInstance().RespawnPlayersAtNextLevel();
        PlayerManager::GetInstance().ShowAllPlayers();
        PlayerManager::GetInstance().ActivateAllPlayers();
    }

     void Game::startNewGame(string type) {
        if(gameStatus == GameStatus::SplashScreen) {
            splashScreen.reset(nullptr);
            splashScreen = nullptr;
        }
        if(type == "oneplayer") {
            gameType = GameType::OnePlayer;
        }
        if(type == "twoplayers") {
            gameType = GameType::TwoPlayers;
        }
        if(type == "pvc") {
            gameType = GameType::PvC;
        }
        LevelManager::GetInstance().ResetLevels();
        LevelManager::GetInstance().LoadNextLevel();
        FruitManager::GetInstance().GenerateFruits(INITIAL_FRUITS);
        PlayerManager::GetInstance().RemoveAllPlayers();
        PlayerManager::GetInstance().CreateGamePlayers(this);
        PlayerManager::GetInstance().ShowAllPlayers();
        PlayerManager::GetInstance().ActivateAllPlayers();
        Gameboard->Show();
        banner->Hide();
        gameStatus = GameStatus::Playing;
     }

    void Game::playerCrashed(shared_ptr<Player> player) {
        if(player->IsDead()) {
            gameStatus = GameStatus::GameOver;
            PlayerManager::GetInstance().DeactivateAllPlayers();
            banner->ClearItems();
            banner->AddItem("lbl1", "GAME OVER!!!");
            if(PlayerManager::GetInstance().GetTotalPlayers() > 1)
                banner->AddItem("lbl2", "WINNER : " + PlayerManager::GetInstance().GetLastPlayerAlive()->Who());
            banner->SetPos((screenWidth / 2) - (banner->GetRect().w / 2), (screenHeight / 2) - (banner->GetRect().h / 2));
            banner->Show();
        } else {
            if(!player->IsHuman()) {
                PlayerManager::GetInstance().RespawnPlayer(player);
                player->Activate();
            }
        }
    }

    void Game::playerAteFruit(shared_ptr<Player> player, Fruit& fruit) {
        FruitManager::GetInstance().GenerateFruits(1);
        Gameboard->ClearRange(fruit.GetBoardCoords());
        FruitManager::GetInstance().RemoveFruit(fruit);
        if(LevelManager::GetInstance().IsLevelCompleted(player->GetLevelScore())) {
            gameStatus = GameStatus::LevelCompleted;
            PlayerManager::GetInstance().DeactivateAllPlayers();
            banner->ClearItems();
            if(LevelManager::GetInstance().IsLevelsFinished()) {
                gameStatus = GameStatus::GameFinished;
                banner->AddItem("lbl1","GAME COMPLETED!!!");
            } else {
                banner->AddItem("lbl1", "LEVEL FINISHED!!!");
            }
            if(PlayerManager::GetInstance().GetTotalPlayers() > 1) {
                PlayerManager::GetInstance().WinForPlayer(player);
                if(gameStatus == GameStatus::GameFinished) {
                    if(PlayerManager::GetInstance().GetFinalWinner())
                        banner->AddItem("lbl2", "FINAL WINNER : " + PlayerManager::GetInstance().GetFinalWinner()->Who());
                    else
                        banner->AddItem("lbl2", "IT IS A DRAW!!!");
                } else {
                    banner->AddItem("lbl2", "LEVEL WINNER : " + player->Who());
                }
            }
            banner->SetPos((screenWidth / 2) - (banner->GetRect().w / 2), (screenHeight / 2) - (banner->GetRect().h / 2));
            banner->Show();
        }
    }

    void Game::Update() {
        PlayerManager::GetInstance().UpdatePlayers();
    }

    void Game::Render() {
        SDL_SetRenderDrawColor(Renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
        SDL_RenderClear(Renderer);
        if(splashScreen != nullptr)
            splashScreen->Render();
        if(Gameboard != nullptr)
            Gameboard->Render();
        FruitManager::GetInstance().RenderAll();
        PlayerManager::GetInstance().RenderPlayers();
        if(banner != nullptr)
            banner->Render();
        if(menu != nullptr)
            menu->Render();
        SDL_RenderPresent(Renderer);
    }

    void Game::clean() {
        SDL_DestroyRenderer(Renderer);
        SDL_Quit();
    }

    bool Game::Running() const {
        return isRunning;
    }

    Game::GameType Game::GetType() const {
        return gameType;
    }

    Game::GameStatus Game::GetStatus() const {
        return gameStatus;
    }

    pair<size_t, size_t> Game::GetScreenSize() const {
        return pair<size_t, size_t>(screenWidth, screenHeight);
    }

}