#pragma once
#include "Game.h"

class StartScreen
{
private:
    bool isSingle = false;
    bool isMulti = false;
    
    SDL_Renderer *rend;
public:
    StartScreen(SDL_Renderer *renderer);
    ~StartScreen();
    void handleEvents(const Uint8 *keystate, bool *isSelecting, bool *isSingle, bool *isMulti);
    void Update();
    void Render();
    void setMode();
};
