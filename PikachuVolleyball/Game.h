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
    bool isMultiConnected = false;
    bool isHost = false;
    bool isGuest = false;
    
    bool isReady = true;
    Uint32 readyStartTime;
    Uint32 currentTime;

    bool playerWon = false;
    
    int targetScore = 3;
    
    TCPsocket server = NULL;
    TCPsocket client = NULL;
    
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
    void checkReadyTime();
    void update();
    void render();
    void reset();
    void clean();
//    void killResource();
    void createPlayers();
    
    bool running();
    bool singleMode();
    bool multiMode();
    bool optionMode();
    
    static SDL_Renderer *renderer;
    static SDL_Surface *screen;
    static SDL_Event event;
};
