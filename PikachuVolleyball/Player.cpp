#include "Player.h"
#include "TextureManager.h"

const int GAME_WIDTH = 800, GAME_HEIGHT = 600;
const float GRAVITY = 0.08;

Player::Player(const char *textureSheet, int w, int h, float x, float y, const char _flag) : GameObject(textureSheet, w, h, x, y)
{
    jumpSound = Mix_LoadWAV("music/jump.ogg");
    dashSound = Mix_LoadWAV("music/dash.ogg");
    spikeSound = Mix_LoadWAV("music/spike.ogg");

    objTexture = TextureManager::LoadTexture(textureSheet);
    
    width = w;
    height = h;
    xpos = x;
    ypos = y;
    radius = 50;

    xVel = 0;
    yVel = 0;
    
    score = 0;
    flag = _flag; // for Left or Right player
    
    // for initial position
    srcRect.w = 400;
    srcRect.h = 400;
    srcRect.x = 0;
    srcRect.y = 0;
    
    destRect.w = width;
    destRect.h = height;
    destRect.x = xpos;
    destRect.y = ypos;
};

Player::~Player()
{};

void Player::MovePressed(const Uint8 *keystate)
{
    
    if(keystate[SDL_SCANCODE_UP] && ypos == GAME_HEIGHT-height) // jump and from ground only
    {
        Mix_PlayChannel(-1, jumpSound, 0);
        yVel = -5;
    }
    
    if(keystate[SDL_SCANCODE_LEFT]) // left
        if((flag == 'L' && xpos > 5) || (flag == 'R' && xpos > GAME_WIDTH/2))
        {
            xVel = -5;
        }
    if(keystate[SDL_SCANCODE_RIGHT]) // right
        if((flag == 'L' && xpos+width < GAME_WIDTH/2) || (flag == 'R' && xpos+width < GAME_WIDTH-5))
        {
            xVel = 5;
        }

    if(keystate[SDL_SCANCODE_LEFT] && keystate[SDL_SCANCODE_SPACE]) // left dash
        if((flag == 'L' && xpos > 0 && ypos + height == GAME_HEIGHT) || (flag == 'R' && xpos > GAME_WIDTH/2 && ypos + height == GAME_HEIGHT))
        {
            objTexture = TextureManager::LoadTexture("images/left_dash_pikachu.png");
            Mix_PlayChannel(-1, dashSound, 0);

            xVel = -7;
            yVel = -2;
        }
    if(keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_SPACE]) // right dash
        if((flag == 'L' && xpos > 0 && ypos + height == GAME_HEIGHT) || (flag == 'R' && xpos > GAME_WIDTH/2 && ypos + height == GAME_HEIGHT))
        {
            objTexture = TextureManager::LoadTexture("images/right_dash_pikachu.png");
            Mix_PlayChannel(-1, dashSound, 0);

            xVel = 7;
            yVel = -2;
        }

    if(keystate[SDL_SCANCODE_LEFT] && keystate[SDL_SCANCODE_LSHIFT]) // left spike
        if((flag == 'L' && xpos > 0) || (flag == 'R' && xpos > 400))
        {
            Mix_PlayChannel(-1, spikeSound, 0);

            GameObject* flame = new GameObject("images/flame.png", 100, 100, GetXpos()-20, GetYpos()-20);
            delete flame;
        }
    if(keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_LSHIFT]) // right spike
        if((flag == 'L' && xpos > 0) || (flag == 'R' && xpos > 400))
        {
            Mix_PlayChannel(-1, spikeSound, 0);

            GameObject* flame = new GameObject("images/flame.png", 100, 100, GetXpos()-20, GetYpos()-20);
            delete flame;
        }

};

void Player::MoveReleased(const Uint8 *keystate)
{
    if(!keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT])
    {
        xVel = 0;
        std::cout << "left or right key released!" << std::endl;
    }
};


void Player::Update()
{
//    previousTime = currentTime;
//    currentTime = SDL_GetTicks();
    
//    float dt = currentTime - previousTime;
//    float dt = 1;

//    xpos += xVel * dt;
//    ypos += yVel * dt;
//    xVel += GRAVITY * dt;
//    yVel += GRAVITY * dt;
    
    xpos += xVel;
    ypos += yVel;
    
    if((flag == 'L' && xpos < 1) || (flag == 'L' && xpos+width > GAME_WIDTH/2))
        xVel = 0;
    if((flag == 'R' && xpos < GAME_WIDTH/2) || (flag == 'R' && xpos+width > GAME_WIDTH))
        xVel = 0;
    
    // Prevent player falling over the ground due to yVel
    if(yVel > 5)
        yVel = 0;
    if(ypos+height > GAME_HEIGHT)
        ypos = GAME_HEIGHT-height;
    // ALWAYS ON GRAVITY
    if(ypos+height < GAME_HEIGHT)
        yVel += GRAVITY;
 
    std::cout << "yPos: " << ypos << std::endl;
    
    if(flag == 'L' && ypos+height == GAME_HEIGHT)
        objTexture = TextureManager::LoadTexture("images/right_look_pikachu.png");
    else if(flag == 'R' && ypos+height == GAME_HEIGHT)
        objTexture = TextureManager::LoadTexture("images/left_look_pikachu.png");
    
    // draw the player
    destRect.x = xpos;
    destRect.y = ypos;
};

int Player::GetScore()
{
    return score;
};

void Player::SetScore(int _score)
{
    score = _score;
};

void Player::Reset(const char _flag)
{
    xVel = 0;
    yVel = 0;

    if(_flag == 'L')
    {
        xpos = 0;
        ypos = GAME_HEIGHT-height;
    }else
    {
        xpos = GAME_WIDTH-width;
        ypos = GAME_HEIGHT-height;
    }
};

float Player::GetXpos()
{
    return xpos;
};

float Player::GetYpos()
{
    return ypos;
};

float Player::GetRadius()
{
    return radius;
};

