#pragma once
#include "Game.h"

class ScreenManager
{
private:
public:
    ScreenManager();
    ~ScreenManager();
    
    static void FadeInAndOut(SDL_Renderer* renderer);
};
