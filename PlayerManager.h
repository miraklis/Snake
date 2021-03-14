#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "stdHeaders.h"
#include "Singleton.h"
#include "SDL.h"
#include "AI.h"

namespace Snake {

	class Game;
	class Player;

	class PlayerManager : public Singleton<PlayerManager>
	{
	public:
		~PlayerManager();
		shared_ptr<Player> CreatePlayer(bool human);
		shared_ptr<Player> GetPlayer(int id);
		shared_ptr<Player> SpawnNewPlayer(bool human);
		void CreateGamePlayers(Game* game);
		void HandlePlayersInput(SDL_Keycode input);
		void RespawnPlayersAtNextLevel();
		void RespawnPlayer(shared_ptr<Player> player);
		void RenderPlayers();
		void UpdatePlayers();
		void RemovePlayer(shared_ptr<Player> player);
		void RemoveAllPlayers();
		void ActivateAllPlayers();
		void DeactivateAllPlayers();
		void PausePlayers();
		void UnPausePlayers();
		void ShowAllPlayers();
		void WinForPlayer(shared_ptr<Player> player);
		shared_ptr<Player> GetLastWinner();
		shared_ptr<Player> GetFinalWinner();
		shared_ptr<Player> GetLastPlayerAlive();
		int GetTotalPlayers() const;
	private:
		friend class Singleton<PlayerManager>;
		PlayerManager();
		vector<shared_ptr<Player>> players;
		shared_ptr<Player> winner;
		void PutPlayerOnBoard(shared_ptr<Player> player);
	};

}

#endif //PLAYERSMANAGER_H