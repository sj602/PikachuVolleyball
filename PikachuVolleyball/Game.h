#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>

class Game
{
private:
    bool isRunning = false;
    bool isSelecting = true;
    bool isSingle = false;
    bool isMulti = false;
    bool isReady = false;
    Mix_Music *bgm;
    SDL_Window *window;
    SDL_Texture *backgroundImage;

public:
    Game();
    ~Game();
    
    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
    bool selectingMode();
    bool settingMultiplay();
    void displayStartScreen();
    void displayMultiplaySettingScreen();
    void handleEvents();
    void update();
    void render();
    void reset();
    void clean();
    
    bool running();
    bool singleMode();
    bool multiMode();
    
    static SDL_Renderer *renderer;
    static SDL_Surface *screen;
    static SDL_Event event;
    
};
