#include "Game.h"
using namespace std;
const int GAME_WIDTH = 800, GAME_HEIGHT = 600;

int main()
{
    // FPS
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    
    // Game Start
    Game *game = new Game();
    game->init("Pikachu Volleyball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT, false);
    
    while(game->selectingMode())
    {
        game->displayStartScreen();
    }
    while(game->running())
    {
        std::cout << "game is running!" << std::endl;

        // FPS
        frameStart = SDL_GetTicks();
        
        if(game->singleMode())
        {
            std::cout << "singleMode running!" << std::endl;
            game->handleEvents();
            game->update();
            game->render();
        }
        if(game->multiMode())
        {
            std::cout << "multiMode running!" << std::endl;

            while(game->settingMultiplay())
            {
                std::cout << "setting multiMode running!" << std::endl;

                game->displayMultiplaySettingScreen();
            }
            game->handleEvents();
            game->update();
            game->render();
        }
        
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
    
    game->clean();
    
    return 0;
}
