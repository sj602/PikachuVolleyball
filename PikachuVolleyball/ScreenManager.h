#pragma once
#include "Game.h"

class ScreenManager
{
private:
public:
    ScreenManager();
    ~ScreenManager();
    
    static void fadeInAndOut(SDL_Renderer* renderer);
};
