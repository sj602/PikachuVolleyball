#include <cmath>
#include "Ball.h"
#include "TextureManager.h"

const int GAME_WIDTH = 800, GAME_HEIGHT = 600;
const int SPIKE_RADIUS = 20;
// for more updates, user can adjust the constant values
const float GRAVITY = 0.1;
const int BASIC_BOUNCE_VEL = -8;
const int LEFT_SPIKE_XVEL = -13, RIGHT_SPIKE_XVEL = 13, DOWN_SPIKE_XVEL = 10, DOWN_SPIKE_YVEL = 20;
const int BASIC_SPIKE_XVEL = 7, BASIC_SPIKE_YVEL = 5, SPIKE_YVEL = -1;
const int ROTATION_ANGLE = 5;
const int RESET_LEFT_XPOS = 60, RESET_RIGHT_XPOS = GAME_WIDTH-60, RESET_YPOS = 60;
const int TRAILING_PREV = 3;
const int TRAILING_PREV_PREV = 6;

// ground y : 535
// pole left x : 398
// pole right x : 408
// pole top y : 290

Ball::Ball(const char *textureSheet, int w, int h, float x, float y) : GameObject(textureSheet, w, h, x, y)
{
    objTexture = TextureManager::LoadTexture(textureSheet);
    trailingTexture1 = TextureManager::LoadTexture("images/ball_alpha.png");
    trailingTexture2 = TextureManager::LoadTexture("images/ball_blue.png");
    flame = new GameObject("images/flame.png", 100, 100, 0, 0, "img");

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
    delete flame;
};

bool Ball::CheckCollision(Player* p1, Player* p2, const Uint8 *keystate)
{
    int ballX = GetXpos()+50;
    int ballY = GetYpos()+50;
    int p1X = p1->GetXpos()+50;
    int p1Y = p1->GetYpos()+50;
    int p2X = p2->GetXpos()+50;
    int p2Y = p2->GetYpos()+50;
    
    double p1RealDistance = sqrt(pow(ballX - p1X, 2) + pow(ballY - p1Y, 2));
    double p1TouchedDistance = GetRadius() + p1->GetRadius();
    double p2RealDistance = sqrt(pow(ballX - p2X, 2) + pow(ballY - p2Y, 2));
    double p2TouchedDistance = GetRadius() + p2->GetRadius();

    //if the ball touches the player..
    if(p1RealDistance <= p1TouchedDistance)
    {
        std::cout << "p1 touched!" << std::endl;
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
        std::cout << "p2 touched!" << std::endl;
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
        if(keystate[SDL_SCANCODE_LSHIFT] && keystate[SDL_SCANCODE_RIGHT])
        {
            flame->Update();
            setTrailing();

            std::cout << "p1RealDistance: " << p1RealDistance << " p1TouchedDistance: " << p1TouchedDistance << std::endl;
            std::cout << "p1 spiked!" << std::endl;
            xVel = RIGHT_SPIKE_XVEL;
            yVel = SPIKE_YVEL;
            return true;
        }
        if(keystate[SDL_SCANCODE_LSHIFT] && keystate[SDL_SCANCODE_DOWN])
        {
            flame->Update();
            setTrailing();

            std::cout << "p1RealDistance: " << p1RealDistance << " p1TouchedDistance: " << p1TouchedDistance << std::endl;
            std::cout << "p1 spiked!" << std::endl;
            xVel = DOWN_SPIKE_XVEL;
            yVel = DOWN_SPIKE_YVEL;
            return true;
        }
        if(keystate[SDL_SCANCODE_LSHIFT])
        {
            flame->Update();
            setTrailing();

            std::cout << "p1RealDistance: " << p1RealDistance << " p1TouchedDistance: " << p1TouchedDistance << std::endl;
            std::cout << "p1 spiked!" << std::endl;
            xVel = BASIC_SPIKE_XVEL;
            yVel = BASIC_SPIKE_YVEL;
            return true;
        }
    }
    
    if(p2RealDistance <= p2TouchedDistance + SPIKE_RADIUS && keystate[SDL_SCANCODE_LSHIFT])
    {
//        Mix_PlayChannel(-1, spikeSound, 0);

        
        return true;
    }
    
    
    // pole left x : 395
    // pole right x : 410
    // pole top y : 290
    
    // if the ball touches the left side of the pole
//    if( ( ( 394.5<GetXpos()+width ) || ( 395.5<GetXpos()+width ) ) && (290<(GetYpos()+height)))
//    if((394.5<GetXpos()+width) && (GetXpos()+width<395.5) && (290<(GetYpos()+height)))
//    {
//        std::cout << "pole left touched!" << std::endl;
//        xVel = -xVel;
//        return true;
//    }
//    // if the ball touches the right side of the pole
//    if((409.5<GetXpos()) && (GetXpos()<410.5) && (290<(GetYpos())))
//    {
//        std::cout << "pole right touched!" << std::endl;
//        xVel = -xVel;
//        return true;
//    }
    if(394<=GetXpos()+width && 396>=GetXpos()+width)
    {
        if(290<=GetYpos()+height)
        {
            std::cout << "pole left touched!" << std::endl;
            std::cout << "GetXpos()+width" << GetXpos()+width << " GetYpos()" << GetYpos() << std::endl;
            xVel = -xVel;
            SetXpos(GetXpos()-3);
            return true;
        }
    }
    // if the ball touches the right side of the pole
    if(409<=GetXpos() && 411>=GetXpos())
    {
        if(290<=GetYpos()+height)
        {
            std::cout << "pole right touched!" << std::endl;
            std::cout << "GetXpos()+width" << GetXpos()+width << " GetYpos()" << GetYpos() << std::endl;
            xVel = -xVel;
            SetXpos(GetXpos()+3);
            return true;
        }
    }

    // if the ball touches the top of the pole
//    if((395<GetXpos()+width) && (410>GetXpos()) && ( (289.8<(GetYpos()+height)) && (290.2<(GetYpos()+height)) ))
//    {
//        std::cout << "pole top touched!" << std::endl;
//        yVel = -yVel;
//        SDL_Delay(1000);
//        std::cout << "GetXpos()+width" << GetXpos()+width << " GetYpos()+height" << GetYpos()+height << std::endl;
//        return true;
//    }

    
//    if(GetXpos() + width < 398 && GetYpos() + height < 291)
//    {
//        xVel = -xVel;
//        yVel = -yVel;
//    }
//    if(GetXpos() < 408 && GetYpos() + height > 290)
//    {
//        xVel = -xVel;
//        yVel = -yVel;
//    }
    
    return false;
};

// check if the ball touches the ground, and then update the score, reset all.
bool Ball::CheckGround(Player* p1, Player* p2, char &winPlayer)
{
    //if the ball touches the ground..
    if(xpos < GAME_WIDTH/2 && ypos + height > GAME_HEIGHT)
    {
        Mix_PlayChannel(-1, groundSound, 0);
        
        //
        p2->SetScore(p2->GetScore()+1);
        winPlayer = 'R';
        return true;
    }else if(xpos > GAME_WIDTH/2 && ypos + height > GAME_HEIGHT)
    {
        Mix_PlayChannel(-1, groundSound, 0);
        
        //
        p1->SetScore(p1->GetScore()+1);
        winPlayer = 'L';
        return true;
    }
    return false;
};

void Ball::Update()
{
    flame->Update();
    std::cout << "isTrailing: " << isTrailing << std::endl;
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
    
    // draw the ball
    destRect.x = xpos;
    destRect.y = ypos;
    
    trailingDestRect1.x = xpos-xVel*TRAILING_PREV;
    trailingDestRect1.y = ypos-yVel*TRAILING_PREV;
    trailingDestRect2.x = xpos-xVel*TRAILING_PREV_PREV;
    trailingDestRect2.y = ypos-yVel*TRAILING_PREV_PREV;
};

void Ball::Reset(const char _flag)
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

void Ball::Render(double angle)
{
    if(isTrailing)
    {
        flame->Render();
        SDL_RenderCopyEx(Game::renderer, trailingTexture1, &srcRect, &trailingDestRect1, angle, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(Game::renderer, trailingTexture2, &srcRect, &trailingDestRect2, angle, NULL, SDL_FLIP_NONE);
    }
    
    SDL_RenderCopyEx(Game::renderer, objTexture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
}

float Ball::GetRadius()
{
    return radius;
};

double Ball::GetAngle()
{
    return angle;
};

void Ball::setTrailing()
{
    Mix_PlayChannel(-1, spikeSound, 0);
    
    isTrailing = true;
};
