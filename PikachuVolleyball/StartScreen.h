#pragma once
#include "Screen.h"

class StartScreen: public Screen
{
public:
    explicit StartScreen(SDL_Renderer *renderer);
    ~StartScreen();
    virtual void handleEvents(const Uint8 *keystate, bool &isSelecting, bool &isSingle, bool &isMulti, bool &isOption);
    virtual void update() override;
    virtual void render() override;
};
