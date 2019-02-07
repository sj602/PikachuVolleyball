#include <cstdio>
#include <cmath>
#include "GameObject.h"
#include "TextureManager.h"

// for general
GameObject::GameObject(const char *textureSheet, int w, int h, int x, int y)
{
    objTexture = TextureManager::LoadTexture(textureSheet);
    
    width = w;
    height = h;
    xpos = x;
    ypos = y;
    
    // for initial position
    srcRect.w = 300;
    srcRect.h = 300;
    srcRect.x = 0;
    srcRect.y = 0;
    
    destRect.w = width;
    destRect.h = height;
    destRect.x = xpos;
    destRect.y = ypos;
};

// for scores
GameObject::GameObject(int score, int w, int h, int x, int y)
{
    // convert int score to char *str
    char str[100];
    sprintf(str, "%d", score);
    objTexture = TextureManager::LoadFont(str);
    
    width = w;
    height = h;
    xpos = x;
    ypos = y;
    
    // for initial position
    srcRect.w = 100;
    srcRect.h = 100;
    srcRect.x = 0;
    srcRect.y = 0;
    
    destRect.w = 70;
    destRect.h = 70;
    destRect.x = xpos;
    destRect.y = ypos;
};

// for background map
GameObject::GameObject(const char *textureSheet, int w, int h)
{
    objTexture = TextureManager::LoadTexture(textureSheet);
    
    width = w;
    height = h;
    xpos = 0;
    ypos = 0;
    
    // for initial position
    srcRect.w = 1280;
    srcRect.h = 537;
    srcRect.x = 0;
    srcRect.y = 0;
    
    destRect.w = width;
    destRect.h = height;
    destRect.x = xpos;
    destRect.y = ypos;
};


void GameObject::Update()
{
    destRect.x = xpos;
    destRect.y = ypos;
};

// score update
void GameObject::Update(int score)
{
    char str[100];
    sprintf(str, "%d", score);
    objTexture = TextureManager::LoadFont(str);

    destRect.x = xpos;
    destRect.y = ypos;
};

void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
};
