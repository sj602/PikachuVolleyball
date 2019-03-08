#include <cmath>
#include "Ball.h"
#include "TextureManager.h"

const int GAME_WIDTH = 800, GAME_HEIGHT = 600;
const int SPIKE_RADIUS = 20;
// for more updates, user can adjust the constant values
const float GRAVITY = 0.1;
const int BASIC_BOUNCE_VEL = -8;
const int LEFT_SPIKE_XVEL = -13, RIGHT_SPIKE_XVEL = 13, DOWN_SPIKE_XVEL = 10, DOWN_SPIKE_YVEL = 20;
const int BASIC_SPIKE_XVEL = 10, BASIC_SPIKE_YVEL = 5, SPIKE_YVEL = -1;
const int ROTATION_ANGLE = 5;
const int RESET_LEFT_XPOS = 60, RESET_RIGHT_XPOS = GAME_WIDTH-60, RESET_YPOS = 60;
const int TRAILING_PREV = 3, TRAILING_PREV_PREV = 6;
const int FLAME_WIDTH = 50, FLAME_HEIGHT = 50;

// ground y : 535
// pole left x : 398
// pole right x : 408
// pole top y : 290

Ball::Ball(const char *textureSheet, int w, int h, float x, float y) : GameObject(textureSheet, w, h, x, y)
{
    objTexture = TextureManager::loadTexture(textureSheet);
    trailingTexture1 = TextureManager::loadTexture("images/ball_alpha.png");
    trailingTexture2 = TextureManager::loadTexture("images/ball_blue.png");
    flame = new GameObject("images/flame.png", FLAME_WIDTH, FLAME_HEIGHT, NULL, NULL, "img");

    groundSound = Mix_LoadWAV("music/ground.ogg");
    spikeSound = Mix_LoadWAV("music/spike.ogg");

    width = w;
    height = h;
    xpos = x;
    ypos = y;
    xVel = 0;
    yVel = 2; // for initial falling
    radius = 50;
    angle = 0;
    
    // for initial position
    srcRect.w = 300;
    srcRect.h = 300;
    srcRect.x = 0;
    srcRect.y = 0;
    
    trailingDestRect2.w = trailingDestRect1.w = destRect.w = width;
    trailingDestRect2.h = trailingDestRect1.h = destRect.h = height;
    destRect.x = xpos;
    destRect.y = ypos;
};

Ball::~Ball()
{
    std::cout << "Ball deconstructor()" << std::endl;
    delete flame;
    SDL_DestroyTexture(trailingTexture1);
    SDL_DestroyTexture(trailingTexture2);
};

bool Ball::checkCollision(Player* p1, Player* p2, const Uint8 *keystate)
{
    int ballX = getXpos()+50;
    int ballY = getYpos()+50;
    int p1X = p1->getXpos()+50;
    int p1Y = p1->getYpos()+50;
    int p2X = p2->getXpos()+50;
    int p2Y = p2->getYpos()+50;
    
    float p1RealDistance = sqrt(pow(ballX - p1X, 2) + pow(ballY - p1Y, 2));
    float p1TouchedDistance = getRadius() + p1->getRadius();
    float p2RealDistance = sqrt(pow(ballX - p2X, 2) + pow(ballY - p2Y, 2));
    float p2TouchedDistance = getRadius() + p2->getRadius();

    //if the ball touches the player..
    if(p1RealDistance <= p1TouchedDistance)
    {
        if(ballX < p1X)
        {
            xVel = (p1X - ballX) / 9;
            xVel = -xVel;
        }else if(ballX > p1X)
        {
            xVel = (ballX - p1X) / 9;
        }
        yVel = BASIC_BOUNCE_VEL;
        isTrailing = false; // after the trailing ball touches the player, it would stop trailing
        
        return true;
    }
    
    if(p2RealDistance <= p2TouchedDistance)
    {
        if(ballX < p2X)
        {
            xVel = (p2X - ballX) / 9;
            xVel = -xVel;
        }else if(ballX > p2X)
        {
            xVel = (ballX - p2X) / 9;
        }
        yVel = BASIC_BOUNCE_VEL;
        isTrailing = false; // after the trailing ball touches the player, it would stop trailing

        return true;
    }
    
    // for spike..
    if(p1RealDistance <= p1TouchedDistance + SPIKE_RADIUS)
    {
        if((keystate[SDL_SCANCODE_LSHIFT] && keystate[SDL_SCANCODE_RIGHT]) || p1->getSpikeState() == 1)
        {
            setFlamePos(ballX, ballY, p1X, p1Y, p2X, p2Y, 'L');
            flameOn = true;
            flameStartTime = SDL_GetTicks();
            flame->update();
            
            setTrailing();

            xVel = RIGHT_SPIKE_XVEL;
            yVel = SPIKE_YVEL;
            return true;
        }
        if((keystate[SDL_SCANCODE_LSHIFT] && keystate[SDL_SCANCODE_DOWN]) || p1->getSpikeState() == 2)
        {
            setFlamePos(ballX, ballY, p1X, p1Y, p2X, p2Y, 'L');
            flameOn = true;
            flameStartTime = SDL_GetTicks();
            flame->update();
            
            setTrailing();

            xVel = DOWN_SPIKE_XVEL;
            yVel = DOWN_SPIKE_YVEL;
            return true;
        }
        if(keystate[SDL_SCANCODE_LSHIFT] || p1->getSpikeState() == 3)
        {
            setFlamePos(ballX, ballY, p1X, p1Y, p2X, p2Y, 'L');
            flameOn = true;
            flameStartTime = SDL_GetTicks();
            flame->update();
            
            setTrailing();

            xVel = BASIC_SPIKE_XVEL;
            yVel = BASIC_SPIKE_YVEL;
            return true;
        }
    }
    
    if(p2RealDistance <= p2TouchedDistance + SPIKE_RADIUS)
    {
        if((keystate[SDL_SCANCODE_LSHIFT] && keystate[SDL_SCANCODE_LEFT]) || p2->getSpikeState() == 1)
        {
            SDL_Delay(100);
            setFlamePos(ballX, ballY, p1X, p1Y, p2X, p2Y, 'R');
            flameOn = true;
            flameStartTime = SDL_GetTicks();
            flame->update();
            
            setTrailing();
            
            xVel = LEFT_SPIKE_XVEL;
            yVel = SPIKE_YVEL;
            return true;
        }
        if((keystate[SDL_SCANCODE_LSHIFT] && keystate[SDL_SCANCODE_DOWN]) || p2->getSpikeState() == 2)
        {
            setFlamePos(ballX, ballY, p1X, p1Y, p2X, p2Y, 'R');
            flameOn = true;
            flameStartTime = SDL_GetTicks();
            flame->update();
            
            setTrailing();
            
            xVel = DOWN_SPIKE_XVEL;
            yVel = DOWN_SPIKE_YVEL;
            return true;
        }
        if(keystate[SDL_SCANCODE_LSHIFT] || p2->getSpikeState() == 3)
        {
            setFlamePos(ballX, ballY, p1X, p1Y, p2X, p2Y, 'R');
            flameOn = true;
            flameStartTime = SDL_GetTicks();
            flame->update();
            
            setTrailing();
            
            xVel = BASIC_SPIKE_XVEL;
            yVel = BASIC_SPIKE_YVEL;
            return true;
        }

        return true;
    }
    
    // if the ball touches the top of the pole
    if(ypos+height>=290 && xpos+width>=395 && xpos<=410)
    {
        setYpos(getYpos()-5);
        yVel = -yVel;
        return true;
    }
    // if the ball touches the left side of the pole
    else if(395<=getXpos()+width && side == 'L')
    {
        if(290<=getYpos()+height)
        {
            xVel = -xVel;
            setXpos(getXpos()-3);
            return true;
        }
    }
    // if the ball touches the right side of the pole
    else if(410>=getXpos() && side == 'R')
    {
        if(290<=getYpos()+height)
        {
            xVel = -xVel;
            setXpos(getXpos()+3);
            return true;
        }
    }
    
    
    return false;
};

// check if the ball touches the ground, and then update the score, reget all.
bool Ball::checkGround(Player* p1, Player* p2, char &winPlayer)
{
    //if the ball touches the ground..
    if(xpos < GAME_WIDTH/2 && ypos + height > GAME_HEIGHT)
    {
        Mix_PlayChannel(-1, groundSound, 0);
        
        //
        p2->setScore(p2->getScore()+1);
        winPlayer = 'R';
        return true;
    }else if(xpos > GAME_WIDTH/2 && ypos + height > GAME_HEIGHT)
    {
        Mix_PlayChannel(-1, groundSound, 0);
        
        //
        p1->setScore(p1->getScore()+1);
        winPlayer = 'L';
        return true;
    }
    return false;
};

void Ball::update(bool& isMulti, bool& isHost, bool& isGuest, TCPsocket& client)
{
    // basic movement inside the window
    xpos += xVel;
    ypos += yVel;
    yVel += GRAVITY;
    
    // basic rotation
    if(xVel > 0)
        angle += ROTATION_ANGLE;
    else if(xVel < 0)
        angle -= ROTATION_ANGLE;
    
    if(xpos <= 0)
    {
        xpos += 5;
        xVel = -xVel;
    }
    if(xpos + width >= GAME_WIDTH)
    {
        xpos -= 5;
        xVel = -xVel;
    }
    if(ypos + height <= 0)
    {
        ypos += 5;
        yVel = -yVel;
    }
    if(ypos + height >= GAME_HEIGHT)
    {
        yVel = -yVel;
    }
    
    // update ball's side to check bar collision
    if(xpos+width < 400)
        side = 'L';
    if(xpos > 400)
        side = 'R';
    
    // draw the ball
    destRect.x = xpos;
    destRect.y = ypos;
    
    trailingDestRect1.x = xpos-xVel*TRAILING_PREV;
    trailingDestRect1.y = ypos-yVel*TRAILING_PREV;
    trailingDestRect2.x = xpos-xVel*TRAILING_PREV_PREV;
    trailingDestRect2.y = ypos-yVel*TRAILING_PREV_PREV;
    
    if(isMulti)
        actMulti(isHost, isGuest, client, data);
};

void Ball::reset(const char _flag)
{
    if(_flag == 'L')
    {
        xpos = RESET_LEFT_XPOS;
    } else if(_flag == 'R')
    {
        xpos = RESET_RIGHT_XPOS-width;
    }
    
    ypos = RESET_YPOS;
    xVel = 0;
    yVel = 2;
    isTrailing = false;
};

void Ball::render(float angle)
{
    if(isTrailing)
    {
        checkFlameTime();
        if(flameOn)
            flame->render();
        
        SDL_RenderCopyEx(Game::renderer, trailingTexture1, &srcRect, &trailingDestRect1, angle, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(Game::renderer, trailingTexture2, &srcRect, &trailingDestRect2, angle, NULL, SDL_FLIP_NONE);
    }
    
    SDL_RenderCopyEx(Game::renderer, objTexture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
}

float Ball::getRadius()
{
    return radius;
};

float Ball::getAngle()
{
    return angle;
};

float Ball::getyVel()
{
    return yVel;
};

void Ball::setTrailing()
{
    Mix_PlayChannel(-1, spikeSound, 0);
    
    isTrailing = true;
};

void Ball::setyVel(float _yVel)
{
    yVel = _yVel;
}

void Ball::checkFlameTime()
{
    currentTime = SDL_GetTicks();
    if(currentTime - flameStartTime > 100) // displaying flame for 0.1sec
        flameOn = false;
}

void Ball::setFlamePos(int ballX, int ballY, int p1X, int p1Y, int p2X, int p2Y, const char flag)
{
    if(flag == 'L')
    {
        if(ballX > p1X)
            flame->setXpos(p1X+(ballX-p1X)/2);
        if(ballX < p1X)
            flame->setXpos(p1X-(p1X-ballX)/2);
        if(ballY > p1Y)
            flame->setYpos(p1Y+(ballY-p1Y)/2);
        if(ballY < p1Y)
            flame->setYpos(p1Y-(p1Y-ballY)/2);
    }
    else
    {
        if(ballX > p2X)
            flame->setXpos(p2X+(ballX-p2X)/2);
        if(ballX < p2X)
            flame->setXpos(p2X-(p2X-ballX)/2);
        if(ballY > p2Y)
            flame->setYpos(p2Y+(ballY-p2Y)/2);
        if(ballY < p2Y)
            flame->setYpos(p2Y-(p2Y-ballY)/2);
    }
}

void Ball::actMulti(bool& isHost, bool& isGuest, TCPsocket& client, ballData* data)
{
    data->b_xpos = xpos;
    data->b_ypos = ypos;
    data->b_xVel = xVel;
    data->b_yVel = yVel;
    data->f_xpos = flame->getXpos();
    data->f_ypos = flame->getYpos();
    data->_flameOn = flameOn;
    
    if(isHost)
    {
        SDLNet_TCP_Send(client, data, 44);
        if( xVel == LEFT_SPIKE_XVEL || xVel == RIGHT_SPIKE_XVEL || xVel == DOWN_SPIKE_XVEL || xVel == BASIC_SPIKE_XVEL ||
           yVel == BASIC_SPIKE_YVEL || yVel == DOWN_SPIKE_YVEL || yVel == SPIKE_YVEL )
        {
            isTrailing = true;
        }
    }
    if(isGuest)
    {
        SDLNet_TCP_Recv(client, data, 44);
        xpos = data->b_xpos;
        ypos = data->b_ypos;
        xVel = data->b_xVel;
        yVel = data->b_yVel;
        flame->setXpos(data->f_xpos);
        flame->setYpos(data->f_ypos);
        
        flameOn = data->_flameOn;
        flameStartTime = SDL_GetTicks();
        flame->update();
        
        if(flameOn)
            setTrailing();
    }
}
