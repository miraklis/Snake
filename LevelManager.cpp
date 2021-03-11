#include "LevelManager.h"
#include "Game.h"
//#include "Board.h"

namespace Snake {

    LevelManager::LevelManager() {
        levelGoals[0] = 5;
        levelGoals[1] = 5;
        levelGoals[2] = 5;
        levelGoals[3] = 5;
        ResetLevels();
    }

    LevelManager::~LevelManager() {
    }

    bool LevelManager::LoadLevel(int lvlID) {
        bool retVal{ false };
        string lvlName = "lvl" + to_string(lvlID) + ".txt";
        ifstream lvl(lvlName);
        if(lvl.is_open()) {
            Game::Gameboard->ClearMatrix();
            string line;
            size_t x, y;
            y = 0;
            while(getline(lvl, line)) {
                x = 0;
                char c;
                while(x < line.length()) {
                    c = line.at(x);
                    Game::Gameboard->SetCell(x, y, c);
                    x++;
                }
                y++;
            }
            currentLevel = lvlID;
            currentGoal = levelGoals[currentLevel - 1];
            lvl.close();
            retVal = true;
        }
        return retVal;
    }

    void LevelManager::LoadNextLevel() {
        if(currentLevel < MAX_LEVEL) {
            LoadLevel(currentLevel + 1);
        }
    }

    int LevelManager::GetLevel() {
        return currentLevel;
    }

    int LevelManager::GetGoal() {
        return currentGoal;
    }

    bool LevelManager::IsLevelCompleted(int score) {
        return (score >= currentGoal);
    }

    void LevelManager::ResetLevels() {
        currentLevel = 0;       
    }

    bool LevelManager::IsLevelsFinished() {
        return (currentLevel >= MAX_LEVEL);
    }

}