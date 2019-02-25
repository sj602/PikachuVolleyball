#include <cstdlib>
#include <ctime>
#include "Game.h"
using namespace std;
const int GAME_WIDTH = 800, GAME_HEIGHT = 600;
// FPS
const int FPS = 60;
const int frameDelay = 1000 / FPS;
Uint32 frameStart;
int frameTime;

int main()
{
    // random setting for AI
    srand((unsigned int)time(NULL));

    Game *game = new Game();
    game->init("Pikachu Volleyball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT, false);
    
    while(game->selectingMode())
    {
        game->displayStartScreen();
        while(game->optionMode())
            game->displayOptionScreen();
    }
    while(game->running())
    {
        frameStart = SDL_GetTicks();
        
        if(game->singleMode())
        {
            game->handleEvents();
            game->update();
            game->render();
            game->checkGameSet();
        }
        if(game->multiMode())
        {
            while(game->settingMultiplay())
            {
                game->displayMultiplaySettingScreen();
            }
            game->handleEvents();
            game->update();
            game->render();
            game->checkGameSet();
        }
        
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
    
    game->clean();
    return 0;
}
