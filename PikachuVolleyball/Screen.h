#pragma once
#include "Game.h"
#include "GameObject.h"

class Screen
{
protected:
    SDL_Renderer *rend = nullptr;
    GameObject *screen = nullptr;
    GameObject *arrow = nullptr;
    
    SDL_Color black = {0, 0, 0, 0xFF};
public:
    Screen();
    explicit Screen(SDL_Renderer *renderer);
    virtual ~Screen();
    virtual void handleEvents(const Uint8 *keystate, bool &isSelecting, bool &isSingle, bool &isMulti){};
    virtual void update() = 0;
    virtual void render() = 0;
};
