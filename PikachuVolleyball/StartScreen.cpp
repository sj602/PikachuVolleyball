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

StartScreen::StartScreen(SDL_Renderer *renderer): Screen(renderer)
{
    std::cout << "StartScreen constructor()!" << std::endl;

    singlePlayMode = new GameObject("SINGLE PLAY", MODE_WIDTH, MODE_HEIGHT, 300, 320);
    multiPlayMode = new GameObject("MULTI PLAY", MODE_WIDTH, MODE_HEIGHT, 300, 380);
    optionMode = new GameObject("OPTIONS", MODE_WIDTH, MODE_HEIGHT, 300, 440);
    title = new GameObject("Pikachu Volleyball", 400, MODE_HEIGHT, 200, 50);
    explanation = new GameObject("Press Enter on any mode..", 400, MODE_HEIGHT, 200, 200);
    copyright = new GameObject("(C) Jinko, All rights reserved", 400, 50, 200, 500);
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

void StartScreen::handleEvents(const Uint8 *keystate, bool &isSelecting, bool &isSingle, bool &isMulti, bool &isOption)
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
    SDL_RenderClear(rend);
    
    // this is where we put things to render
    screen->render();
    singlePlayMode->render();
    multiPlayMode->render();
    optionMode->render();
    title->render();
    explanation->render();
    copyright->render();
    arrow->render();
    
    SDL_RenderPresent(rend);
}

