#include "StartScreen.h"
#include "GameObject.h"
#include "TextureManager.h"

const int MODE_WIDTH = 150;
const int MODE_HEIGHT = 50;

GameObject *singlePlayMode;
GameObject *multiPlayMode;
GameObject *optionMode;
GameObject *title;
GameObject *explanation;
GameObject *copyright;

StartScreen::StartScreen(SDL_Renderer* _renderer): Screen(_renderer)
{
    std::cout << "StartScreen constructor()!" << std::endl;

    singlePlayMode = new GameObject("SINGLE PLAY", MODE_WIDTH, MODE_HEIGHT, 300, 320, white);
    multiPlayMode = new GameObject("MULTI PLAY", MODE_WIDTH, MODE_HEIGHT, 300, 380, white);
    optionMode = new GameObject("OPTIONS", MODE_WIDTH, MODE_HEIGHT, 300, 440, white);
    title = new GameObject("Pikachu Volleyball", 400, MODE_HEIGHT, 200, 50, white);
    explanation = new GameObject("Press Enter on any mode..", 400, MODE_HEIGHT, 200, 200, white);
    copyright = new GameObject("(C) Jinko, All rights reserved", 400, 50, 200, 500, white);
}

StartScreen::~StartScreen()
{
    std::cout << "StartScreen deconstructor()!" << std::endl;

    delete singlePlayMode;
    delete multiPlayMode;
    delete optionMode;
    delete title;
    delete explanation;
    delete copyright;
}

void StartScreen::handleEvents(const Uint8*& keystate, bool &isSelecting, bool &isSingle, bool &isMulti, bool &isOption)
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    
    switch (event.type) {
        case SDL_KEYDOWN:
            if(keystate[SDL_SCANCODE_DOWN] && arrow->getYpos() == 390)
            {
                arrow->setYpos(arrow->getYpos()+60);
            }
            if(keystate[SDL_SCANCODE_DOWN] && arrow->getYpos() == 330)
            {
                arrow->setYpos(arrow->getYpos()+60);
            }
            if(keystate[SDL_SCANCODE_UP] && arrow->getYpos() == 390)
            {
                arrow->setYpos(arrow->getYpos()-60);
            }
            if(keystate[SDL_SCANCODE_UP] && arrow->getYpos() == 450)
            {
                arrow->setYpos(arrow->getYpos()-60);
            }
            if(keystate[SDL_SCANCODE_RETURN])
            {
                if(arrow->getYpos() == 330)
                {
                    isSingle = true;
                    isSelecting = false;
                }
                if(arrow->getYpos() == 390)
                {
                    isMulti = true;
                    isSelecting = false;
                }
                if(arrow->getYpos() == 450)
                    isOption = true;
            }
            break;
            
        default:
            break;
    }
}

void StartScreen::update()
{
    screen->update();
    singlePlayMode->update();
    multiPlayMode->update();
    optionMode->update();
    title->update();
    explanation->update();
    copyright->update();
    arrow->update();
}

void StartScreen::render()
{
    SDL_RenderClear(renderer);
    
    // this is where we put things to render
    screen->render();
    singlePlayMode->render();
    multiPlayMode->render();
    optionMode->render();
    title->render();
    explanation->render();
    copyright->render();
    arrow->render();
    
    SDL_RenderPresent(renderer);
}

