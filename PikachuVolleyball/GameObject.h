#pragma once
#include <string>
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
    GameObject(const std::string textureSheet, int w, int h, int x, int y); // for general
    GameObject(int score, int w, int h, int x, int y); // for scores
    GameObject(const char *text, int w, int h, int x, int y); // for texts in StartScreen
    GameObject(const char *textureSheet, int w, int h, int x, int y, const std::string _flag); // for imgs
    virtual ~GameObject(){};
    
    // getters
    float GetXpos();
    float GetYpos();
    // setters
    void SetXpos(float x);
    void SetYpos(float y);
    
    void Update();
    void Update(int score);
    void Render();
    virtual void Move(const Uint8 *keystate){};
};
