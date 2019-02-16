#include "Screen.h"

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;
const int ARROW_WIDTH = 20;
const int ARROW_HEIGHT = 20;

Screen::Screen(SDL_Renderer *renderer)
{
    std::cout << "(rendered)Screen constructor()!" << std::endl;

    rend = renderer;
    screen = new GameObject("images/startscreen.png", GAME_WIDTH, GAME_HEIGHT, 0, 0, "img");
    arrow = new GameObject("images/arrow.png", 30, 30, 250, 330, "img");
}

Screen::Screen()
{
    std::cout << "(default)Screen constructor()!" << std::endl;
}

Screen::~Screen()
{
    std::cout << "Screen deconstrouctor()" << std::endl;
}
