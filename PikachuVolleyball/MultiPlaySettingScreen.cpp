#include "GameObject.h"
#include "MultiplaySettingScreen.h"

GameObject *host;
GameObject *guest;
GameObject *ipInput;
GameObject *portInput;

MultiplaySettingScreen::MultiplaySettingScreen()
{
    std::cout << "MultiplaySettingScreen constructor()!" << std::endl;

    host = new GameObject("Host", 400, 400, 200, 200);
    guest = new GameObject("Guest", 400, 400, 300, 200);
    ipInput = new GameObject("Connecting IP: ", 400, 200, 200, 200);
    portInput = new GameObject("Connecting PORT: ", 400, 50, 200, 500);
    SDL_StartTextInput();
    
    if (SDLNet_Init() == -1)
        std::cout << "SDLNET init failed" << std::endl;
    IPaddress ip;

}

MultiplaySettingScreen::~MultiplaySettingScreen()
{
    std::cout << "MultiplaySettingScreen deconstructor()!" << std::endl;

    SDLNet_Quit();

    delete host;
    delete guest;
    delete ipInput;
    delete portInput;
}

void MultiplaySettingScreen::handleEvents(const Uint8 *keystate, bool *isSelecting, bool *isSingle, bool *isMulti)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    
    switch (event.type) {
        case SDL_KEYDOWN:
            if(keystate[SDL_SCANCODE_DOWN] && arrow->GetYpos() == 330)
            {
                arrow->SetYpos(arrow->GetYpos()+60);
            }
            if(keystate[SDL_SCANCODE_UP] && arrow->GetYpos() == 390)
            {
                arrow->SetYpos(arrow->GetYpos()-60);
            }
            if(keystate[SDL_SCANCODE_RETURN])
            {
                if(arrow->GetYpos() == 330)
                    *isSingle = true;
                if(arrow->GetYpos() == 390)
                    *isMulti = true;
                *isSelecting = false;
            }
            break;
            
        default:
            break;
    }
}

void MultiplaySettingScreen::Update()
{
    screen->Update();
    arrow->Update();
}

void MultiplaySettingScreen::Render()
{
    SDL_RenderClear(rend);
    
    // this is where we put things to render
    screen->Render();
    arrow->Render();
    
    SDL_RenderPresent(rend);
}


