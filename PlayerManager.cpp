#include "PlayerManager.h"
#include "Player.h"
#include "Game.h"

namespace Snake {

	PlayerManager::PlayerManager() {
	}

	PlayerManager::~PlayerManager() {
		players.clear();
	}

	shared_ptr<Player> PlayerManager::CreatePlayer(bool human) {
		int id = players.size();
		string imgFile{"assets/snake.png"};
		if(id==1)
			imgFile =  "assets/snake2.png";
		players.push_back(make_shared<Player>(imgFile, id, human));
		return players[players.size() - 1];
	}

	shared_ptr<Player> PlayerManager::GetPlayer(int id) {
		for(auto& player : players) {
			if(player->GetId() == id) {
				return player;
			}
		}
		return nullptr;
	}

	shared_ptr<Player> PlayerManager::SpawnNewPlayer(bool human) {
		shared_ptr<Player> player = CreatePlayer(human);
		PutPlayerOnBoard(player);
		return player;
	}

	void PlayerManager::CreateGamePlayers(Game* game) {
		SDL_Rect boardRect = Game::Gameboard->GetRect();
		pair<size_t, size_t> screenSize = game->GetScreenSize();
		shared_ptr<Player> newPlayer;
		// Player 1
		newPlayer = PlayerManager::GetInstance().SpawnNewPlayer(true);
		newPlayer->MapKeys(SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN);
		newPlayer->OnCrashedEvent.AddListener(bind(&Game::playerCrashed, game, placeholders::_1));
		newPlayer->OnFruitEatenEvent.AddListener(bind(&Game::playerAteFruit, game, placeholders::_1, placeholders::_2));
		newPlayer->CreateInfoPanel(0, 0, boardRect.x, boardRect.h / 2, 
		                           DARK_RED_COLOR, WHITE_COLOR, 
		                           SDL_GUI::HorizontalAlign::Left, SDL_GUI::VerticalAlign::Top);
		// Player 2
		Game::GameType gameType = game->GetType();
		if(gameType == Game::GameType::TwoPlayers || gameType == Game::GameType::PvC) {
			if(gameType == Game::GameType::PvC) {
				newPlayer = PlayerManager::GetInstance().SpawnNewPlayer(false);
			} else {
				newPlayer = PlayerManager::GetInstance().SpawnNewPlayer(true);
				newPlayer->MapKeys(SDLK_a, SDLK_d, SDLK_w, SDLK_s);
			}
			newPlayer->OnCrashedEvent.AddListener(bind(&Game::playerCrashed, game, placeholders::_1));
			newPlayer->OnFruitEatenEvent.AddListener(bind(&Game::playerAteFruit, game, placeholders::_1, placeholders::_2));
			newPlayer->CreateInfoPanel(boardRect.x + boardRect.w, 0, screenSize.first - (boardRect.x + boardRect.w), boardRect.h / 2, 
			                            DARK_RED_COLOR, WHITE_COLOR, 
			                            SDL_GUI::HorizontalAlign::Right, SDL_GUI::VerticalAlign::Top);
		}
	}

	void PlayerManager::HandlePlayersInput(SDL_Keycode input) {
		for(auto& player : players) {
			player->HandleKeyInput(input);
		}
	}

	void PlayerManager::RespawnPlayersAtNextLevel() {
		for(auto& player : players) {
			player->Init(Player::InitLevel::NewLevel);
			PutPlayerOnBoard(player);
		}
	}

	void PlayerManager::RespawnPlayer(shared_ptr<Player> player) {
		player->Init(Player::InitLevel::Respawn);
		player->EraseFromBoard();
		PutPlayerOnBoard(player);
	}

	void PlayerManager::RenderPlayers() {
		for (auto& player : players) {
			player->Render();
		}
	}

	void PlayerManager::UpdatePlayers() {
		for(auto& player : players) {
			player->Update();
		}
	}

	void PlayerManager::RemovePlayer(shared_ptr<Player> player) {
		players.erase(remove(players.begin(), players.end(), player), players.end());
	}

	void PlayerManager::RemoveAllPlayers() {
		players.clear();
	}

	void PlayerManager::ActivateAllPlayers() {
		for(auto& player : players) {
			player->Activate();
		}
	}

	void PlayerManager::DeactivateAllPlayers() {
		for(auto& player : players) {
			player->Deactivate();
		}
	}

	void PlayerManager::PausePlayers() {
		for(auto& player : players) {
			player->Pause();
		}
	}

	void PlayerManager::UnPausePlayers() {
		for(auto& player : players) {
			player->UnPause();
		}
	}

	void PlayerManager::ShowAllPlayers() {
		for(auto& player : players) {
			player->Show();
		}
	}

	void PlayerManager::WinForPlayer(shared_ptr<Player> player) {
		player->MakeAWin();
		winner = player;
	}

	shared_ptr<Player> PlayerManager::GetFinalWinner() {
		if(players[0]->GetWins() > players[1]->GetWins())
			return players[0];
		if(players[0]->GetWins() < players[1]->GetWins())
			return players[1];
		return nullptr; // No Winner, it's a Draw
	}

	shared_ptr<Player> PlayerManager::GetLastPlayerAlive() {
		if(players[0]->IsDead())
			return players[1];
		return players[0];
	}

	int PlayerManager::GetTotalPlayers() const {
		return players.size();
	}

	shared_ptr<Player> PlayerManager::GetLastWinner() {
		return winner;
	}

	void PlayerManager::PutPlayerOnBoard(shared_ptr<Player> player) {
		vector<pair<int, int>> initPos = Game::Gameboard->GetRegionEmpty(1, 10);
		player->PutOnBoard(initPos[2].first, initPos[2].second); // 3rd cell from the 10cell region to fill with TBH (tail,body,head)
	}
}