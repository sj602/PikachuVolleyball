#pragma once
#include "Game.h"

class GameObject
{
protected:
    int width;
    int height;
    float xpos;
    float ypos;
    float radius;
    
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;

public:
    GameObject(const char* textureSheet, int w, int h, int x, int y); // for general
    GameObject(int score, int w, int h, int x, int y); // for scores
    GameObject(const char* textureSheet, int w, int h); // for background map
    virtual ~GameObject(){};
    
    void Update();
    void Update(int score);
    void Render();
    virtual void Move(const Uint8 *keystate){};
};
