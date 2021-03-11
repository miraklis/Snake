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

	shared_ptr<Player> PlayerManager::GetLastWinner() {
		return winner;
	}

	void PlayerManager::PutPlayerOnBoard(shared_ptr<Player> player) {
		vector<pair<int, int>> initPos = Game::Gameboard->GetRegionEmpty(1, 10);
		player->PutOnBoard(initPos[2].first, initPos[2].second); // 3rd cell from the 20cell region to fill with TBH (tail,body,head)
	}
}