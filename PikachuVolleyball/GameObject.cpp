#include <string>
#include <cstdio>
#include <cmath>
#include "GameObject.h"
#include "TextureManager.h"

const int IMG_WIDTH = 1280;
const int IMG_HEIGHT = 537;
const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;

// for general
GameObject::GameObject(const std::string textureSheet, int w, int h, int x, int y)
{
    const char *texture = textureSheet.c_str();
    objTexture = TextureManager::LoadTexture(texture);
    
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
    
    destRect.w = width;
    destRect.h = height;
    destRect.x = xpos;
    destRect.y = ypos;
};

// for texts in StartScreen
GameObject::GameObject(const char *text, int w, int h, int x, int y)
{
    // convert int score to char *str
    char str[100];
    sprintf(str, "%s", text);
    objTexture = TextureManager::LoadFont(str);
    
    width = w;
    height = h;
    xpos = x;
    ypos = y;
    
    // for initial position
    srcRect.w = width;
    srcRect.h = height;
    srcRect.x = 0;
    srcRect.y = 0;
    
    destRect.w = width;
    destRect.h = height;
    destRect.x = xpos;
    destRect.y = ypos;
};


// for imgs
GameObject::GameObject(const char *textureSheet, int w, int h, int x, int y, const std::string _flag)
{
    if(_flag == "img")
    {
        objTexture = TextureManager::LoadTexture(textureSheet);
        
        width = w;
        height = h;
        xpos = x;
        ypos = y;
        
        // for initial position
        srcRect.w = 1600;
        srcRect.h = 1600;
        //    srcRect.w = IMG_WIDTH;
        //    srcRect.h = IMG_HEIGHT;
        //    srcRect.w = GAME_WIDTH;
        //    srcRect.h = GAME_HEIGHT;
        srcRect.x = 0;
        srcRect.y = 0;
        
        destRect.w = width;
        destRect.h = height;
        destRect.x = xpos;
        destRect.y = ypos;
    }
};

float GameObject::GetXpos()
{
    return xpos;
}

float GameObject::GetYpos()
{
    return ypos;
}

void GameObject::SetXpos(float x)
{
    xpos = x;
}

void GameObject::SetYpos(float y)
{
    ypos = y;
}

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
