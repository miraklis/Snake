#include "TextureManager.h"
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Utils.h"

namespace Snake {
    SDL_Texture* LoadTexture(string texFile) {
        SDL_Surface* tmpSurface = IMG_Load(texFile.c_str());
        if(!tmpSurface) {
            Utils::ErrorMsg("LOADING IMAGE", SDL_GetError());
            return nullptr;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::Renderer, tmpSurface);
        if(!texture) {
            Utils::ErrorMsg("CREATING TEXTURE", SDL_GetError());
            return nullptr;
        }
        return texture;
    }
}