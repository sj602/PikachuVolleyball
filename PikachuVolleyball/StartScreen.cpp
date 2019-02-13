#include "StartScreen.h"
#include "GameObject.h"
#include "TextureManager.h"

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;
const int MODE_WIDTH = 150;
const int MODE_HEIGHT = 50;
const int ARROW_WIDTH = 20;
const int ARROW_HEIGHT = 20;

GameObject *screen;
GameObject *singlePlayMode;
GameObject *multiPlayMode;
GameObject *title;
GameObject *explaination;
GameObject *copyRight;
GameObject *arrow;

StartScreen::StartScreen(SDL_Renderer *renderer)
{
    rend = renderer;
    screen = new GameObject("images/startscreen.png", GAME_WIDTH, GAME_HEIGHT, 0, 0, "img");
    singlePlayMode = new GameObject("SINGLE PLAY", MODE_WIDTH, MODE_HEIGHT, 300, 320);
    multiPlayMode = new GameObject("MULTI PLAY", MODE_WIDTH, MODE_HEIGHT, 300, 380);
    title = new GameObject("Pikachu Volleyball", 400, MODE_HEIGHT, 200, 50);
    explaination = new GameObject("Press Enter on any mode..", 400, MODE_HEIGHT, 200, 200);
    copyRight = new GameObject("(C) Jinko, All rights reserved", 400, 50, 200, 500);
    arrow = new GameObject("images/arrow.png", 30, 30, 250, 330, "img");
}

StartScreen::~StartScreen()
{
    delete singlePlayMode;
    delete multiPlayMode;
    delete title;
    delete explaination;
    delete copyRight;
    delete arrow;
    delete screen;
}

void StartScreen::handleEvents(const Uint8 *keystate, bool *isSelecting, bool *isSingle, bool *isMulti)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    
    switch (event.type) {
        case SDL_KEYDOWN:
            if(keystate[SDL_SCANCODE_DOWN] && arrow->GetYpos() == 330)
                arrow->SetYpos(arrow->GetYpos()+60);
            if(keystate[SDL_SCANCODE_UP] && arrow->GetYpos() == 390)
                arrow->SetYpos(arrow->GetYpos()-60);
            if(keystate[SDL_SCANCODE_RETURN])
            {
                if(arrow->GetYpos() == 330)
                    *isSingle = true;
                if(arrow->GetYpos() == 390)
                    *isMulti = true;
                *isSelecting = false;
            }
            break;
            
        default:
            break;
    }
}

void StartScreen::Update()
{
    screen->Update();
    singlePlayMode->Update();
    multiPlayMode->Update();
    title->Update();
    explaination->Update();
    copyRight->Update();
    arrow->Update();
}

void StartScreen::Render()
{
    SDL_RenderClear(rend);
    
    // this is where we put things to render
    screen->Render();
    singlePlayMode->Render();
    multiPlayMode->Render();
    title->Render();
    explaination->Render();
    copyRight->Render();
    arrow->Render();
    
    SDL_RenderPresent(rend);
}

