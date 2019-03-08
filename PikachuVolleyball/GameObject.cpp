#include <string>
#include <cstdio>
#include <cmath>
#include "GameObject.h"
#include "TextureManager.h"

const int IMG_WIDTH = 1280;
const int IMG_HEIGHT = 537;
const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;

GameObject::~GameObject()
{
    std::cout << objTexture << " deconstructor()" << std::endl;
}

// for general
GameObject::GameObject(const std::string textureSheet, int w, int h, int x, int y)
{
    const char *texture = textureSheet.c_str();
    objTexture = TextureManager::loadTexture(texture);
    
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
    objTexture = TextureManager::loadFont(str);
    
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

// for texts (red is basic color)
GameObject::GameObject(const char *text, int w, int h, int x, int y)
{
    objTexture = TextureManager::loadFont(text);
    
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

// for texts in custom color
GameObject::GameObject(const std::string text, int w, int h, int x, int y, SDL_Color& textColor)
{
    objTexture = TextureManager::loadFont(text, textColor);
    
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
GameObject::GameObject(const char *textureSheet, int w, int h, int x, int y, const std::string& _flag)
{
    if(_flag == "img")
    {
        objTexture = TextureManager::loadTexture(textureSheet);
        
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

float GameObject::getXpos()
{
    return xpos;
}

float GameObject::getYpos()
{
    return ypos;
}

void GameObject::setXpos(float x)
{
    xpos = x;
}

void GameObject::setYpos(float y)
{
    ypos = y;
}

void GameObject::setText(const std::string& text, SDL_Color& textColor)
{
    SDL_DestroyTexture(objTexture);
    objTexture = TextureManager::loadFont(text, textColor);
}

void GameObject::update()
{
    destRect.x = xpos;
    destRect.y = ypos;
};

// score update
void GameObject::update(int score)
{
    char str[100];
    sprintf(str, "%d", score);
    SDL_DestroyTexture(objTexture);
    objTexture = TextureManager::loadFont(str);

    destRect.x = xpos;
    destRect.y = ypos;
};

void GameObject::render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
};
