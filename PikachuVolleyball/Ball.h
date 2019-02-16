#pragma once
#include "GameObject.h"
#include "Player.h"

class Ball: public GameObject
{
private:
    float xVel;
    float yVel;
    double angle;
    
    bool isTrailing = false;
    SDL_Texture* trailingTexture1;
    SDL_Texture* trailingTexture2;
    SDL_Rect trailingDestRect1;
    SDL_Rect trailingDestRect2;
    
    GameObject* flame = nullptr;

public:
    Ball(const char* textureSheet, int w, int h, float x, float y);
    ~Ball();
    
    void Update();
    bool CheckCollision(Player* p1, Player* p2, const Uint8 *keystate);
    bool CheckGround(Player* p1, Player* p2, char &winPlayer);
    void Reset(const char _flag);
    void Render(double angle);
    void setTrailing();

    // getters
    float GetRadius();
    double GetAngle();
    
    // move to private later on..
    Mix_Chunk *groundSound;
    Mix_Chunk *spikeSound;
};
