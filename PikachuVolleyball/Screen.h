#pragma once
#include "Game.h"
#include "GameObject.h"

class Screen
{
protected:
    SDL_Renderer *renderer = nullptr;
    GameObject *screen = nullptr;
    GameObject *arrow = nullptr;
    
    SDL_Color black = {0, 0, 0, 0xFF};
    SDL_Color white = {255, 255, 255, 0xFF};
    
public:
    Screen();
    explicit Screen(SDL_Renderer* _renderer);
    virtual ~Screen();
    virtual void handleEvents(const Uint8*& keystate, bool &isSelecting, bool &isSingle, bool &isMulti){};
    virtual void update(){};
    virtual void render(){};
};
