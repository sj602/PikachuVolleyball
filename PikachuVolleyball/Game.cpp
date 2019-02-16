#include "Game.h"
#include "StartScreen.h"
#include "MultiplaySettingScreen.h"
#include "TextureManager.h"
#include "ScreenManager.h"
#include "GameObject.h"
#include "Player.h"
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
//GameObject *flame;

SDL_Renderer* Game::renderer = nullptr;
const Uint8 *keystate = SDL_GetKeyboardState(NULL);

//Screen *startScreen = nullptr;
//Screen *multiSettingScreen = nullptr;
StartScreen *startScreen = nullptr;
MultiplaySettingScreen *multiSettingScreen = nullptr;

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
    multiSettingScreen = new MultiplaySettingScreen();
    player1 = new Player("images/right_look_pikachu.png", PLAYER_WIDTH, PLAYER_HEIGHT, 0, GAME_HEIGHT-PLAYER_HEIGHT, 'L');
    player2 = new Player("images/left_look_pikachu.png", PLAYER_WIDTH, PLAYER_HEIGHT, GAME_WIDTH-PLAYER_WIDTH, GAME_HEIGHT-PLAYER_HEIGHT, 'R');
    map = new GameObject("images/background.png", GAME_WIDTH, GAME_HEIGHT, 0, 0, "img");
    pole = new GameObject("images/pole.png", 9, 300, 400, 380);
    ball = new Ball("images/ball.png", BALL_WIDTH, BALL_HEIGHT, 60, 60);
    score1 = new GameObject(0, SCORE_WIDTH, SCORE_HEIGHT, 0, 0);
    score2 = new GameObject(0, SCORE_WIDTH, SCORE_HEIGHT, 750, 0);
//    flame = new GameObject("images/flame.png", 30, 30, 0, 0, "img");
    
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
    startScreen->handleEvents(keystate, &isSelecting, &isSingle, &isMulti);
    startScreen->Update();
    startScreen->Render();
    
    if(isSelecting == false)
        if(Mix_PlayMusic(bgm, -1) < 0)
            cout << "could not play music! SDL_mixer Error: " << Mix_GetError() << endl;
};

void Game::displayMultiplaySettingScreen()
{
    multiSettingScreen->handleEvents(keystate, &isSelecting, &isSingle, &isMulti);
    multiSettingScreen->Update();
    multiSettingScreen->Render();
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
            player1->MovePressed(keystate);
//            player2->MovePressed(keystate);
            break;
            
        case SDL_KEYUP:
            player1->MoveReleased(keystate);
//            player2->MoveReleased(keystate);
            break;
            
        case SDL_MOUSEMOTION:
            int x, y;
            SDL_GetMouseState(&x, &y);
//            std::cout << "mouse x: " << x << " mouse y: " << y << std::endl;
            
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);
            ball->SetXpos(x);
            ball->SetYpos(y);
            break;
            
        default:
            break;
    }
};

void Game::update()
{
    map->Update();
    player1->Update();
    player2->Update();
    
    ball->Update();
    ball->CheckCollision(player1, player2, keystate);
    if(ball->CheckGround(player1, player2, winPlayer))
    {
        // trying to fade in and out when ball touches ground...
        SDL_Texture *fadingScreen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
        SDL_SetTextureAlphaMod(fadingScreen, 0);
        SDL_RenderCopy(renderer, fadingScreen, NULL, NULL);
        SDL_RenderPresent(renderer);

        //        ScreenManager::FadeInAndOut(renderer);
        
        if(winPlayer == 'L')
            ball->Reset('L');
        if(winPlayer == 'R')
            ball->Reset('R');
        player1->Reset('L');
        player2->Reset('R');
        
    }
    
    score1->Update(player1->GetScore());
    score2->Update(player2->GetScore());
//    flame->Update();
};

void Game::render()
{
    SDL_RenderClear(renderer);

    // this is where we put things to render
    map->Render();
    player1->Render();
    player2->Render();
    ball->Render(ball->GetAngle());
    score1->Render();
    score2->Render();
//    flame->Render();
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, ball->GetXpos()+50, ball->GetYpos()+50, player1->GetXpos()+50, player1->GetYpos()+50);
    
    SDL_RenderPresent(renderer);
};

void Game::reset()
{
    SDL_Delay(1000);
    /*
     1. 공이 땅바닥에 닿는다
     2. 게임이 느려진다
     3. 화면이 암전된다
     4. 모든 것이 리셋된다
     5. ready?
     6. start..
     */
};

void Game::clean()
{
    Mix_HaltMusic();
    Mix_FreeMusic(bgm);
//    Mix_FreeChunk(Player::jumpSound);
//    Mix_FreeChunk(Player::spikeSound);
//    Mix_FreeChunk(Player::dashSound);

    delete startScreen;
    delete multiSettingScreen;
    delete player1;
    delete player2;
    delete map;
    delete ball;
    delete pole;
    delete score1;
    delete score2;
//    delete flame;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    cout << "Game is cleaned" << endl;
};

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

