#include "Player.h"
#include "TextureManager.h"

const int GAME_WIDTH = 800, GAME_HEIGHT = 600;
const float GRAVITY = 0.3;
const int JUMP_YVEL = -13;
const int LEFT_MOVE_XVEL = -4, RIGHT_MOVE_XVEL = 4;
const int LEFT_DASH_XVEL = -7, RIGHT_DASH_XVEL = 7, DASH_YVEL = -4;

Player::Player(const char *textureSheet, int w, int h, float x, float y, const char _flag) : GameObject(textureSheet, w, h, x, y)
{
    jumpSound = Mix_LoadWAV("music/jump.ogg");
    dashSound = Mix_LoadWAV("music/dash.ogg");

    objTexture = TextureManager::loadTexture(textureSheet);
    
    width = w;
    height = h;
    xpos = x;
    ypos = y;
    radius = 50;

    xVel = 0;
    yVel = 0;
    score = 0;
    
    flag = _flag; // for Left or Right or AI player
    if(flag == 'A')
    {
        setxVel(LEFT_MOVE_XVEL);
        name = "Computer";
    }
    if(flag == 'L')
        name = "Player1";
    if(flag == 'R')
        name = "Player2";

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
{
    std::cout << "Player deconstructor()" << std::endl;
};

void Player::movePressed(const Uint8 *keystate)
{
    if(keystate[SDL_SCANCODE_UP] && ypos == GAME_HEIGHT-height) // jump and from ground only
    {
        Mix_PlayChannel(-1, jumpSound, 0);
        yVel = JUMP_YVEL;
    }
    
    if(keystate[SDL_SCANCODE_LEFT] && !isDashing) // left
        if((flag == 'L' && xpos > 5) || (flag == 'R' && xpos > GAME_WIDTH/2))
        {
            xVel = LEFT_MOVE_XVEL;
        }
    if(keystate[SDL_SCANCODE_RIGHT] && !isDashing) // right
        if((flag == 'L' && xpos+width < GAME_WIDTH/2) || (flag == 'R' && xpos+width < GAME_WIDTH-5))
        {
            xVel = RIGHT_MOVE_XVEL;
        }

    if((keystate[SDL_SCANCODE_LEFT] && keystate[SDL_SCANCODE_SPACE])) // left dash
        if((flag == 'L' && xpos > 0 && ypos + height == GAME_HEIGHT) || (flag == 'R' && xpos > GAME_WIDTH/2 && ypos + height == GAME_HEIGHT))
        {
            SDL_DestroyTexture(objTexture);
            objTexture = TextureManager::loadTexture("images/left_dash_pikachu.png");
            Mix_PlayChannel(-1, dashSound, 0);
            xVel = -1; // dummy value for handling dashing
            yVel = DASH_YVEL;

            isDashing = true;
        }
    if((keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_SPACE])) // right dash
        if((flag == 'L' && xpos > 0 && ypos + height == GAME_HEIGHT) || (flag == 'R' && xpos > GAME_WIDTH/2 && ypos + height == GAME_HEIGHT))
        {
            SDL_DestroyTexture(objTexture);
            objTexture = TextureManager::loadTexture("images/right_dash_pikachu.png");
            Mix_PlayChannel(-1, dashSound, 0);
            xVel = 1; // dummy value for handling dashing
            yVel = DASH_YVEL;
            isDashing = true;
        }

    if((keystate[SDL_SCANCODE_LEFT] && keystate[SDL_SCANCODE_LSHIFT]) || didSpike) // left spike
        if((flag == 'L' && xpos > 0) || (flag == 'R' && xpos > 400))
        {
            GameObject* flame = new GameObject("images/flame.png", 100, 100, getXpos()-20, getYpos()-20);
            delete flame;
        }
    if((keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_LSHIFT]) || didSpike) // right spike
        if((flag == 'L' && xpos > 0) || (flag == 'R' && xpos > 400))
        {
            GameObject* flame = new GameObject("images/flame.png", 100, 100, getXpos()-20, getYpos()-20);
            delete flame;
        }
 
};

void Player::moveReleased(const Uint8 *keystate)
{
    if(!keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT])
    {
        xVel = 0;
    }
};

void Player::update(bool& isMulti, bool& isHost, bool& isGuest, TCPsocket& client)
{
    xpos += xVel;
    ypos += yVel;
    
    if((flag == 'L' && xpos < 1))
    {
        xpos = 0;
        xVel = 0;
    }
    if((flag == 'L' && xpos+width > GAME_WIDTH/2))
    {
        xpos = (GAME_WIDTH/2)-width;
        xVel = 0;
    }
    if((flag == 'R' && xpos < GAME_WIDTH/2))
    {
        xpos = GAME_WIDTH/2;
        xVel = 0;
    }
    if((flag == 'R' && xpos+width > GAME_WIDTH))
    {
        xpos = GAME_WIDTH-width;
        xVel = 0;
    }
    
    // Prevent player falling over the ground
    if(ypos+height > GAME_HEIGHT)
    {
        ypos = GAME_HEIGHT-height;
        yVel = 0;
    }
    // ALWAYS ON GRAVITY
    if(ypos+height < GAME_HEIGHT)
        yVel += GRAVITY;
    
    // Dashing
    if(isDashing)
    {
        if(xVel < 0)
            xVel = LEFT_DASH_XVEL;
        if(xVel > 0)
            xVel = RIGHT_DASH_XVEL;
    }
    if(isDashing && yVel == 0) // once if dash, it won't stop until the player is on the ground.
    {
        xVel = 0;
        isDashing = false;
    }
     
    if(flag == 'L' && ypos+height == GAME_HEIGHT)
    {
        SDL_DestroyTexture(objTexture);
        objTexture = TextureManager::loadTexture("images/right_look_pikachu.png");
    }
    else if(flag == 'R' && ypos+height == GAME_HEIGHT)
    {
        SDL_DestroyTexture(objTexture);
        objTexture = TextureManager::loadTexture("images/left_look_pikachu.png");
    }

    // draw the player
    destRect.x = xpos;
    destRect.y = ypos;
    
    // for AI's random spike 
    if(didSpike)
        didSpike = false;
    
    if(flag == 'A')
        actAI();
    if(isMulti)
        actMulti(isHost, isGuest, client);
};

int Player::getScore()
{
    return score;
};

void Player::setScore(const int _score)
{
    score = _score;
};

void Player::setFlag(const char _flag)
{
    flag = _flag;
}

void Player::setxVel(const float _xVel)
{
    xVel = _xVel;
}

void Player::reset(const char _flag)
{
    xVel = 0;
    yVel = 0;

    if(_flag == 'L')
    {
        xpos = 0;
        ypos = GAME_HEIGHT-height;
    }else if(_flag == 'R')
    {
        xpos = GAME_WIDTH-width;
        ypos = GAME_HEIGHT-height;
    }else if(_flag == 'A')
    {
        xVel = LEFT_MOVE_XVEL;
        xpos = GAME_WIDTH-width;
        ypos = GAME_HEIGHT-height;
    }
};

float Player::getRadius()
{
    return radius;
};

char Player::getFlag()
{
    return flag;
};

std::string Player::getName()
{
    return name;
};

int Player::getSpikeState()
{
    // this function returns 0,1,2,3 randomly
    // 0 for nothing
    // 1 for straight spike
    // 2 for down spike
    // 3 for drop spike
    if(didSpike)
    {
        int random = rand() % 3 + 1;
        return random;
    }
    return 0;
}

void Player::actAI()
{
    if(xpos < GAME_WIDTH/2)
    {
        xpos = GAME_WIDTH/2;
        xVel = RIGHT_MOVE_XVEL;
    }
    if(xpos+width > GAME_WIDTH)
    {
        xpos = GAME_WIDTH-width;
        xVel = LEFT_MOVE_XVEL;
    }
    
    // jump randomly(at 1/50 chances)
    int randomJump = rand() % 50 + 1;
    if(randomJump == 1 && ypos == GAME_HEIGHT-height)
        yVel = JUMP_YVEL;
    
    // spike randomly(at 1/10 chances)
    int randomSpike = rand() % 10 + 1;
    if(randomSpike == 1 && !didSpike)
        didSpike = true;
};

void Player::actMulti(bool& isHost, bool& isGuest, TCPsocket& client)
{
    data->_xpos = xpos;
    data->_ypos = ypos;
    data->_xVel = xVel;
    data->_yVel = yVel;
    data->_isDashing = isDashing;
    
    if(isHost)
    {
        if(flag == 'L')
        {
            // me
            SDLNet_TCP_Send(client, data, 24);
        }
        if(flag == 'R')
        {
            // opponent
            SDLNet_TCP_Recv(client, data, 24);
            xpos = data->_xpos;
            ypos = data->_ypos;
            xVel = data->_xVel;
            yVel = data->_yVel;
            isDashing = data->_isDashing;
        }
    }
    if(isGuest)
    {
        if(flag == 'L')
        {
            // opponent
            SDLNet_TCP_Recv(client, data, 24);
            xpos = data->_xpos;
            ypos = data->_ypos;
            xVel = data->_xVel;
            yVel = data->_yVel;
            isDashing = data->_isDashing;
        }
        if(flag == 'R')
        {
            // me
            SDLNet_TCP_Send(client, data, 24);
        }
    }
};

