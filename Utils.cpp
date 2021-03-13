#include "Utils.h"
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"

namespace Snake {

    void Msg(string msg) {
        cout << "INFO: " << msg << endl;
    }

    void ErrorMsg(string msg, string err) {
        cout << "ERROR: " << msg << ":::" << err << endl;
    }

    SDL_Texture* LoadTexture(string texFile) {
        SDL_Surface* tmpSurface = IMG_Load(texFile.c_str());
        if(!tmpSurface) {
            ErrorMsg("LOADING IMAGE", SDL_GetError());
            return nullptr;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::Renderer, tmpSurface);
        if(!texture) {
            ErrorMsg("CREATING TEXTURE", SDL_GetError());
            return nullptr;
        }
        return texture;
    }
}