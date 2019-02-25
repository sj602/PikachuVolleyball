#pragma once
#include "GameObject.h"
#include "Player.h"

class Ball: public GameObject
{
private:
    float xVel;
    float yVel;
    float angle;
    
    bool isTrailing = false;
    SDL_Texture* trailingTexture1;
    SDL_Texture* trailingTexture2;
    SDL_Rect trailingDestRect1;
    SDL_Rect trailingDestRect2;
    
    bool flameOn = false;
    Uint32 flameStartTime;
    Uint32 currentTime;
    GameObject* flame = nullptr;
    
    char side;
public:
    Ball(const char* textureSheet, int w, int h, float x, float y);
    ~Ball();
    
    void update() override;
    bool checkCollision(Player* p1, Player* p2, const Uint8 *keystate);
    bool checkGround(Player* p1, Player* p2, char &winPlayer);
    void reset(const char _flag);
    void render(float angle);
    void checkFlameTime();
    void setFlamePos(int ballX, int ballY, int p1X, int p1Y, int p2X, int p2Y);

    // getters
    float getRadius();
    float getAngle();
    // setters
    void setTrailing();
    void setyVel(float _yVel);
    
    // move to private later on..
    Mix_Chunk *groundSound;
    Mix_Chunk *spikeSound;
};
