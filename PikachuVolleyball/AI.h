#pragma once
#include "GameObject.h"

class AI: public GameObject
{
private:
    float xVel;
    float yVel;
    
    int score;
    char flag;
    bool isDashing = false;
    
public:
    AI(const char* textureSheet, int w, int h, float x, float y, const char _flag);
    ~AI();
    
    void update();
    void reset(const char _flag);
    void movePressed(const Uint8 *keystate);
    void moveReleased(const Uint8 *keystate);
    
    // getters
    int getScore();
    float getRadius();
    
    // setters
    void setScore(int _score);
    
    Uint32 previousTime = 0;
    Uint32 currentTime = SDL_GetTicks();
};
