#pragma once
#include "Game.h"

class TextureManager
{
private:
public:
    static SDL_Texture* LoadTexture(const char *fileName);
    static SDL_Texture* LoadFont(const char *str);
};
