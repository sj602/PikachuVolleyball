#pragma once
#include "Screen.h"

class OptionScreen: public Screen
{
private:
    std::string score;
    bool isInitial = true;
    bool isValid = true;
public:
    explicit OptionScreen(SDL_Renderer *renderer, int& targetScore);
    ~OptionScreen();
    void handleEvents(const Uint8 *keystate, bool& isOption, int& targetScore);
    virtual void update() override;
    virtual void render() override;
    bool checkValid();
};
