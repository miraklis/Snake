#pragma once

#include "stdHeaders.h"
#include "SDL.h"

namespace Snake {
    //class TextureManager {
    //    public:
            //static unique_ptr<SDL_Texture, SDL_Deleter> LoadTexture(string file);
            extern SDL_Texture* LoadTexture(string file);
    //};
}