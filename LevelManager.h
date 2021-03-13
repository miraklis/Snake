#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "stdHeaders.h"
#include "SDL.h"
#include "Singleton.h"

namespace Snake {

    class LevelManager : public Singleton<LevelManager> {
        public:
            ~LevelManager();
            static const size_t MAX_LEVEL {4};
            bool LoadLevel(int lvlID);
            void LoadNextLevel();
            void ResetLevels();
            int GetLevel();
            int GetGoal();
            bool IsLevelCompleted(int score);
            bool IsLevelsFinished();
        private:
            friend class Singleton<LevelManager>;
            LevelManager();
            int currentLevel;
            int currentGoal;
            int levelGoals[MAX_LEVEL];
            bool levelsFinished;
    };
}

#endif //LEVELMANAGER_H