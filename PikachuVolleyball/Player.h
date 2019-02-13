#pragma once
#include "GameObject.h"

class Player : public GameObject
{
private:
    float xVel;
    float yVel;
        
    int score;
    char flag;
    
public:
    Player(const char* textureSheet, int w, int h, float x, float y, const char _flag);
    ~Player();
    
    void Update();
    int GetScore();
    void SetScore(int _score);
    void Reset(const char _flag);
    void MovePressed(const Uint8 *keystate);
    void MoveReleased(const Uint8 *keystate);

    // getters
    float GetRadius();
    
    // move to private later on...
    Mix_Chunk *jumpSound;
    Mix_Chunk *dashSound;
    Mix_Chunk *spikeSound;
    
    Uint32 previousTime = 0;
    Uint32 currentTime = SDL_GetTicks();
};
