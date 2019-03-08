#include "Screen.h"

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;
const int ARROW_WIDTH = 30;
const int ARROW_HEIGHT = 30;

Screen::Screen(SDL_Renderer* _renderer)
{
    std::cout << "(rendered)Screen constructor()!" << std::endl;

    renderer = _renderer;
    screen = new GameObject("images/startscreen.png", GAME_WIDTH, GAME_HEIGHT, 0, 0, "img");
    arrow = new GameObject("images/arrow.png", ARROW_WIDTH, ARROW_HEIGHT, 250, 330, "img");
}

Screen::Screen()
{
    std::cout << "(default)Screen constructor()!" << std::endl;
}

Screen::~Screen()
{
    std::cout << "Screen deconstrouctor()" << std::endl;
    delete screen;
    delete arrow;
}
