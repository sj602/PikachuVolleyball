#pragma once
#include "GameObject.h"
#include "Player.h"

// for data in multiplay
typedef struct {
    float b_xpos;
    float b_ypos;
    float b_xVel;
    float b_yVel;
    float f_xpos;
    float f_ypos;
    bool _flameOn = false;
    bool _isTrailing = false;
    float tb1_xpos;
    float tb1_ypos;
    float tb2_xpos;
    float tb2_ypos;
}ballData;

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
    
    ballData* data = new ballData();
public:
    Ball(const char* textureSheet, int w, int h, float x, float y);
    ~Ball();
    
    void update(bool& isMulti, bool& isHost, bool& isGuest, TCPsocket& client);
    bool checkCollision(Player* p1, Player* p2, const Uint8 *keystate);
    bool checkGround(Player* p1, Player* p2, char &winPlayer);
    void reset(const char _flag);
    void render(float angle);
    void checkFlameTime();
    void setFlamePos(int ballX, int ballY, int p1X, int p1Y, int p2X, int p2Y, const char flag);
    void actMulti(bool& isHost, bool& isGuest, TCPsocket& client, ballData* data);

    // getters
    float getRadius();
    float getAngle();
    float getyVel();
    // setters
    void setTrailing();
    void setyVel(float _yVel);
    
    // move to private later on..
    Mix_Chunk *groundSound;
    Mix_Chunk *spikeSound;
};
