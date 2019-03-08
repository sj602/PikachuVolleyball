#pragma once
#include "Game.h"

class TextureManager
{
private:
public:
    static SDL_Texture* loadTexture(const std::string& fileName);
    static SDL_Texture* loadFont(const std::string& text);
    static SDL_Texture* loadFont(const std::string& text, SDL_Color& textColor);
};
