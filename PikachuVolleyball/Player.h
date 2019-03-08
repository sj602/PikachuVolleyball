#pragma once
#include <SDL2/SDL_net.h>
#include "GameObject.h"

// for data in multiplay
typedef struct {
    float _xpos;
    float _ypos;
    float _xVel;
    float _yVel;
    bool _isDashing;
    bool _didSpike;
}playerData;

class Player : public GameObject
{
private:
    float xVel;
    float yVel;
        
    int score;
    char flag;
    
    std::string name;
    
    bool isDashing = false;
    bool didSpike = false;
    
    playerData* data = new playerData();
    
public:
    Player(const char* textureSheet, int w, int h, float x, float y, const char _flag);
    ~Player();
    
    void update(bool& isMulti, bool& isHost, bool& isGuest, TCPsocket& client);
    void reset(const char _flag);
    void movePressed(const Uint8 *keystate);
    void moveReleased(const Uint8 *keystate);
    
    void actAI();
    void actMulti(bool& isHost, bool& isGuest, TCPsocket& client);

    // getters
    int getScore();
    float getRadius();
    char getFlag();
    int getSpikeState();
    std::string getName();
    
    // setters
    void setScore(const int _score);
    void setFlag(const char _flag);
    void setxVel(const float _xVel);
    
    // move to private later on...
    Mix_Chunk *jumpSound;
    Mix_Chunk *dashSound;
    
    Uint32 previousTime = 0;
    Uint32 currentTime = SDL_GetTicks();
};
