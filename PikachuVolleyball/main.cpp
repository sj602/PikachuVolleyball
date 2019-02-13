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
        // FPS
        frameStart = SDL_GetTicks();
        
        game->handleEvents();
        game->update();
        game->render();
        
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
    
    game->clean();
    
    return 0;
}
