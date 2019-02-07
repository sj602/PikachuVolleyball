#include "ScreenManager.h"

ScreenManager::ScreenManager()
{
    
};

ScreenManager::~ScreenManager()
{
    
};

void ScreenManager::FadeInAndOut(SDL_Renderer* renderer)
{
    SDL_Rect fadingScreen;
    fadingScreen.x = 0;
    fadingScreen.y = 0;
    fadingScreen.w = 800;
    fadingScreen.y = 800;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &fadingScreen);
};
