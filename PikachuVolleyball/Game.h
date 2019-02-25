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
    bool isOption = false;
    bool isSingle = false;
    bool isMulti = false;
    bool isReady = false;
    bool playerWon = false;
    
    int targetScore = 3;
    
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
    void displayOptionScreen();
    void handleEvents();
    void checkGameSet();
    void update();
    void render();
    void reset();
    void clean();
//    void killResource();
    
    bool running();
    bool singleMode();
    bool multiMode();
    bool optionMode();
    
    static SDL_Renderer *renderer;
    static SDL_Surface *screen;
    static SDL_Event event;
};
