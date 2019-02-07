#pragma once
#include "GameObject.h"

class Ball: public GameObject
{
private:
    float xVel;
    float yVel;
//    int angle;
    double angle;
    
public:
    Ball(const char* textureSheet, int w, int h, float x, float y);
    ~Ball();
    
    void Update();
    void Move();
    bool CheckCollision(Player* p1, Player* p2);
    bool CheckGround(Player* p1, Player* p2);
    void Reset(const char _flag);
    void Render(double angle);

    // getters
    float GetXpos();
    float GetYpos();
    float GetRadius();
    double GetAngle();
    
    //setters
    void SetXpos(float x);
    void SetYpos(float y);
    
    // move to private later on..
    Mix_Chunk *groundSound;
};
