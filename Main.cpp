#include "stdHeaders.h"
#include "SDL.h"
#include "Game.h"

using namespace Snake;

int main(int argc, char* argv[])
 {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    uint32_t frameStart;
    int frameTime;

    Game* game = new Game();
    game->Init();

    while(game->Running()) {
        frameStart = SDL_GetTicks();
        
        game->HandleEvents();
        game->Update();
        game->Render();
        
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    delete game;
    return 0;
}