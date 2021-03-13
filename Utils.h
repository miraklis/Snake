#ifndef UTILS_H
#define UTILS_H

#include "stdHeaders.h"
#include "SDL.h"

namespace Snake {
    extern void Msg(string msg);
    extern void ErrorMsg(string msg, string err);
    extern SDL_Texture* LoadTexture(string file);
}

#endif //UTILS_H