#pragma once
#include "GameObject.h"

class Player : public GameObject
{
private:
    float xVel;
    float yVel;
        
    int score;
    char flag;
    
    std::string name;
    
    bool isDashing = false;
public:
    Player(const char* textureSheet, int w, int h, float x, float y, const char _flag);
    ~Player();
    
    void update() override;
    void reset(const char _flag);
    void movePressed(const Uint8 *keystate);
    void moveReleased(const Uint8 *keystate);
    void actAI();

    // getters
    int getScore();
    float getRadius();
    char getFlag();
    std::string getName();
    
    // setters
    void setScore(int _score);
    
    // move to private later on...
    Mix_Chunk *jumpSound;
    Mix_Chunk *dashSound;
    
    Uint32 previousTime = 0;
    Uint32 currentTime = SDL_GetTicks();
};
