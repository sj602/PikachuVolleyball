#include <cmath>
#include "Player.h"
#include "Ball.h"
#include "TextureManager.h"

const int GAME_WIDTH = 800, GAME_HEIGHT = 600;
// for more updates, user can adjust the gravity value
const float GRAVITY = 0.06;
// ground y : 535
// pole left x : 398
// pole right x : 408
// pole top y : 290

Ball::Ball(const char *textureSheet, int w, int h, float x, float y) : GameObject(textureSheet, w, h, x, y)
{
    objTexture = TextureManager::LoadTexture(textureSheet);
    groundSound = Mix_LoadWAV("music/ground.ogg");

    width = w;
    height = h;
    xpos = x;
    ypos = y;
    xVel = 0;
    yVel = 2;
    radius = 50;
    angle = 0;

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

Ball::~Ball()
{};

void Ball::Move()
{
    /*
     1. when pikachu touches the ball, it moves -20 ~ +20
     2. when the ball touches left side of the pole, it moves 300 ~ 360
     3. when the ball touches right side of the pole, it moves 0 ~ 60
     4. when pikachu spike the ball in higher height than the ball, it is moved in high speed
     */
};

bool Ball::CheckCollision(Player* p1, Player* p2)
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

//    std::cout << "ball x: " << GetXpos() << " player1 x: " << p1->GetXpos() << std::endl;
//    std::cout << "ball y: " << GetYpos() << " player1 y: " << p1->GetYpos() << std::endl;
//    std::cout << "ball x: " << GetXpos() << " player2 x: " << p2->GetXpos() << std::endl;
//    std::cout << "ball y: " << GetYpos() << " player2 y: " << p2->GetYpos() << std::endl;
//
//    std::cout << "real distance: " << p1RealDistance << " touched distance: " << p1TouchedDistance << std::endl;

    //if the ball touches the player..
    if(p1RealDistance <= p1TouchedDistance)
    {
        std::cout << "p1 touched!" << std::endl;
        if(ballX < p1X)
        {
            xVel = (p1X - ballX) / 8.5;
            xVel = -xVel;
        }else if(ballX > p1X)
        {
            xVel = (ballX - p1X) / 8.5;
        }
        yVel = -yVel;
        
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
        yVel = -yVel;
        
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
bool Ball::CheckGround(Player* p1, Player* p2)
{
    //if the ball touches the ground..
    if(xpos < 400 && ypos + height > 600)
    {
        Mix_PlayChannel(-1, groundSound, 0);
        p2->SetScore(p2->GetScore()+1);
        this->Reset('R');
        p1->Reset('L');
        return true;
    }else if(xpos > 400 && ypos + height > 600)
    {
        Mix_PlayChannel(-1, groundSound, 0);
        p1->SetScore(p1->GetScore()+1);
        this->Reset('L');
        p2->Reset('R');
        return true;
    }
    return false;
};

void Ball::Update()
{
    // basic movement inside the window
    xpos += xVel;
    ypos += yVel;
    yVel += GRAVITY;
    
    // basic rotation
    if(xVel > 0)
        angle += 5;
    else if(xVel < 0)
        angle -= 5;
    
    if(xpos < 0)
    {
        xVel = -xVel;
    }
    if(xpos + width > GAME_WIDTH)
    {
        xVel = -xVel;
    }
    if(ypos + height < 0)
    {
        yVel = -yVel;
    }
    if(ypos + height > GAME_HEIGHT)
    {
        yVel = -yVel;
    }
    
    // draw the ball
    destRect.x = xpos;
    destRect.y = ypos;

};

void Ball::Reset(const char _flag)
{
    if(_flag == 'L')
    {
        xpos = 60;
    } else if(_flag == 'R')
    {
        xpos = GAME_WIDTH-width-60;
    }
    
    ypos = 60;
    xVel = 0;
    yVel = 2;
};

void Ball::Render(double angle)
{
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
