#include "Game.h"
#include "StartScreen.h"
#include "MultiplaySettingScreen.h"
#include "OptionScreen.h"
#include "TextureManager.h"
#include "ScreenManager.h"
#include "GameObject.h"
#include "Player.h"
#include "AI.h"
#include "Ball.h"

using namespace std;

const int GAME_WIDTH = 800, GAME_HEIGHT = 600;
const int PLAYER_WIDTH = 100, PLAYER_HEIGHT = 100;
const int BALL_WIDTH = 100, BALL_HEIGHT = 100;
const int SCORE_WIDTH = 50, SCORE_HEIGHT = 50;
char winPlayer = NULL;

// for more research, difference between pointing derived and base class, for base class pointer // maybe for polymurphism?
//GameObject *player1;
//GameObject *player2;
//GameObject *ball;
////////////////////
Player *player1;
Player *player2;
Ball *ball;
GameObject *map;
GameObject *pole;
GameObject *score1;
GameObject *score2;
GameObject *wonSign;

SDL_Renderer* Game::renderer = nullptr;
const Uint8 *keystate = SDL_GetKeyboardState(NULL);

//Screen *startScreen = nullptr;
//Screen *multiSettingScreen = nullptr;
StartScreen *startScreen = nullptr;
MultiplaySettingScreen *multiSettingScreen = nullptr;
OptionScreen *optionScreen = nullptr;

Game::Game(){};
Game::~Game(){};

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if(fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;
    
    // SDL Init
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        cout << "SDL Not initialized: " << SDL_GetError() << endl;

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if(!window)
        cout << "window Not created: " << SDL_GetError() << endl;
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
        cout << "renderer Not created: " << SDL_GetError() << endl;
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    
    // Music Init
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
    else
    {
        bgm = Mix_LoadMUS("music/bgm.ogg");
        Mix_VolumeMusic(70);
    }
    
    if(!(IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG))
        cout << "Could not create window: " << IMG_GetError() << endl;
    
    startScreen = new StartScreen(renderer);
    multiSettingScreen = new MultiplaySettingScreen(renderer);
    optionScreen = new OptionScreen(renderer, targetScore);
    player1 = new Player("images/right_look_pikachu.png", PLAYER_WIDTH, PLAYER_HEIGHT, 0, GAME_HEIGHT-PLAYER_HEIGHT, 'L');
    player2 = new Player("images/left_look_pikachu.png", PLAYER_WIDTH, PLAYER_HEIGHT, GAME_WIDTH-PLAYER_WIDTH, GAME_HEIGHT-PLAYER_HEIGHT, 'A');
    map = new GameObject("images/background.png", GAME_WIDTH, GAME_HEIGHT, 0, 0, "img");
    pole = new GameObject("images/pole.png", 9, 300, 400, 380);
    ball = new Ball("images/ball.png", BALL_WIDTH, BALL_HEIGHT, 60, 60);
    score1 = new GameObject(0, SCORE_WIDTH, SCORE_HEIGHT, 0, 0);
    score2 = new GameObject(0, SCORE_WIDTH, SCORE_HEIGHT, 750, 0);
    
    isRunning = true;
};

bool Game::selectingMode()
{
    return isSelecting;
};

bool Game::settingMultiplay()
{
    return true;
};

void Game::displayStartScreen()
{
    startScreen->handleEvents(keystate, isSelecting, isSingle, isMulti, isOption);
    startScreen->update();
    startScreen->render();
    
    if(isSelecting == false)
        if(Mix_PlayMusic(bgm, -1) < 0)
            cout << "could not play music! SDL_mixer Error: " << Mix_GetError() << endl;
};

void Game::displayMultiplaySettingScreen()
{
    multiSettingScreen->handleEvents(keystate, isSelecting, isSingle, isMulti);
    multiSettingScreen->update();
    multiSettingScreen->render();
};

void Game::displayOptionScreen()
{
    optionScreen->handleEvents(keystate, isOption, targetScore);
    optionScreen->update();
    optionScreen->render();
};

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
            
        case SDL_KEYDOWN:
            player1->movePressed(keystate);
//            player2->MovePressed(keystate);
            break;
            
        case SDL_KEYUP:
            player1->moveReleased(keystate);
//            player2->MoveReleased(keystate);
            break;
            
        case SDL_MOUSEMOTION:
            int x, y;
            SDL_GetMouseState(&x, &y);
//            std::cout << "mouse x: " << x << " mouse y: " << y << std::endl;
            
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);
            ball->setXpos(x);
            ball->setYpos(y);
            break;
            
        default:
            break;
    }
};

void Game::update()
{
    player1->update();
    player2->update();
    
    ball->update();
    ball->checkCollision(player1, player2, keystate);
    if(ball->checkGround(player1, player2, winPlayer))
    {
        // trying to fade in and out when ball touches ground...
//        SDL_Texture *fadingScreen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
//        SDL_SetTextureAlphaMod(fadingScreen, 0);
//        SDL_RenderCopy(renderer, fadingScreen, NULL, NULL);
//        SDL_RenderPresent(renderer);

        //        ScreenManager::FadeInAndOut(renderer);
        
        if(winPlayer == 'L')
            ball->reset('L');
        if(winPlayer == 'R')
            ball->reset('R');
        player1->reset('L');
        if(player2->getFlag() == 'A')
            player2->reset('A');
        if(player2->getFlag() == 'R')
            player2->reset('R');
        
        score1->update(player1->getScore());
        score2->update(player2->getScore());
    }
    
    if(playerWon)
        wonSign->update();
};

void Game::render()
{
    SDL_RenderClear(renderer);

    // this is where we put things to render
    map->render();
    player1->render();
    player2->render();
    ball->render(ball->getAngle());
    score1->render();
    score2->render();
    
    if(playerWon)
        wonSign->render();

    SDL_RenderPresent(renderer);
};

void Game::reset()
{
    /*
     1. 공이 땅바닥에 닿는다
     2. 게임이 느려진다
     3. 화면이 암전된다
     4. 모든 것이 리셋된다
     5. ready?
     6. start..
     */
};

void Game::checkGameSet()
{
    if(player1->getScore() == targetScore)
    {
        playerWon = true;
        if(!wonSign)
        {
            const char *name = (player1->getName() + " won!").c_str();
            wonSign = new GameObject(name, 500, 100, 140, 60);
        }
        ball->setyVel(0);
    }
    
    if(player2->getScore() == targetScore)
    {
        playerWon = true;
        if(!wonSign)
        {
            const char *name = (player2->getName() + " won!").c_str();
            wonSign = new GameObject(name, 500, 100, 140, 60);
        }
        ball->setyVel(0);
    }
};

void Game::clean()
{
    Mix_HaltMusic();
    Mix_FreeMusic(bgm);
//    Mix_FreeChunk(Player::jumpSound);
//    Mix_FreeChunk(Ball::groundSound);
//    Mix_FreeChunk(Ball::spikeSound);
//    Mix_FreeChunk(Player::dashSound);

//    SDL_DestroyTexture(backgroundImage);
    
    delete startScreen;
    delete multiSettingScreen;
    delete optionScreen;
    delete player1;
    delete player2;
    delete map;
    delete ball;
    delete pole;
    delete score1;
    delete score2;


    if(multiSettingScreen->getServer())
        SDLNet_TCP_Close(multiSettingScreen->getServer());
    if(multiSettingScreen->getClient())
        SDLNet_TCP_Close(multiSettingScreen->getClient());
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    cout << "Game is cleaned" << endl;
};

//void Game::killResource()
//{
//    if(isSelecting)
//        delete startScreen;
//
//    delete multiSettingScreen;
//    delete optionScreen;
//    delete player1;
//    delete player2;
//    delete map;
//    delete ball;
//    delete pole;
//    delete score1;
//    delete score2;
//}

bool Game::running()
{
    return isRunning;
};

bool Game::singleMode()
{
    return isSingle;
};

bool Game::multiMode()
{
    return isMulti;
};

bool Game::optionMode()
{
    return isOption;
};

