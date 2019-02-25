#include "GameObject.h"
#include "MultiplaySettingScreen.h"

const int MODE_WIDTH = 150, MODE_HEIGHT = 50;
const int TEXT_WIDTH = 300, TEXT_HEIGHT = 50;

GameObject *host;
GameObject *guest;
GameObject *ipInput;
GameObject *ipValue;
GameObject *portInput;
GameObject *portValue;
GameObject *connect;
GameObject *waiting;

MultiplaySettingScreen::MultiplaySettingScreen(SDL_Renderer *renderer): Screen(renderer)
{
    std::cout << "MultiplaySettingScreen constructor()!" << std::endl;

    host = new GameObject("Host mode", MODE_WIDTH, MODE_HEIGHT, 300, 330);
    guest = new GameObject("Guest mode", MODE_WIDTH, MODE_HEIGHT, 300, 390);
    ipInput = new GameObject("Connecting IP: ", MODE_WIDTH, MODE_HEIGHT, 100, 200);
    ipValue = new GameObject(connectingIp, TEXT_WIDTH, TEXT_HEIGHT, 300, 200, black);
    portInput = new GameObject("Connecting PORT: ", MODE_WIDTH, MODE_HEIGHT, 100, 250);
    portValue = new GameObject(connectingPort, MODE_WIDTH, MODE_HEIGHT, 300, 250, black);
    waiting = new GameObject("Waiting...", MODE_WIDTH, MODE_HEIGHT, 300, 500);
    connect = new GameObject("Connect!", MODE_WIDTH, MODE_HEIGHT, 300, 500);
    
}

MultiplaySettingScreen::~MultiplaySettingScreen()
{
    std::cout << "MultiplaySettingScreen deconstructor()!" << std::endl;
    
    SDL_StopTextInput();
    SDLNet_Quit();

    delete host;
    delete guest;
    delete ipInput;
    delete ipValue;
    delete portInput;
    delete portValue;
    delete waiting;
    delete connect;
}

void MultiplaySettingScreen::handleEvents(const Uint8 *keystate, bool &isSelecting, bool &isSingle, bool &isMulti)
{
    SDL_Event event;
    SDL_WaitEvent(&event);

    switch (event.type) {
        case SDL_KEYDOWN:
            if(!isHost && !isGuest)
            {
                if(keystate[SDL_SCANCODE_DOWN] && arrow->getYpos() == 330)
                {
                    arrow->setYpos(arrow->getYpos()+60);
                }
                if(keystate[SDL_SCANCODE_UP] && arrow->getYpos() == 390)
                {
                    arrow->setYpos(arrow->getYpos()-60);
                }
                if(keystate[SDL_SCANCODE_RETURN])
                {
                    if(arrow->getYpos() == 330)
                        isHost = true;
                    if(arrow->getYpos() == 390)
                        isGuest = true;
                }
            }
            if(isGuest)
            {
                arrow->setXpos(60);
                arrow->setYpos(210);
                if(arrow->getYpos() == 210 && keystate[SDL_SCANCODE_BACKSPACE] && connectingIp.length() > 0)
                {
                    connectingIp = connectingIp.substr(0, connectingIp.length()-1);
                    ipValue->setText(connectingIp, black);
                    break;
                }
                if(keystate[SDL_SCANCODE_DOWN] && arrow->getYpos() == 210)
                {
                    arrow->setYpos(260);
                    break;
                }
                if(keystate[SDL_SCANCODE_UP] && arrow->getYpos() == 260)
                {
                    arrow->setYpos(210);
                    break;
                }
                if(keystate[SDL_SCANCODE_DOWN] && arrow->getYpos() == 260)
                {
                    arrow->setXpos(270);
                    arrow->setYpos(510);
                    break;
                }
                if(keystate[SDL_SCANCODE_UP] && arrow->getYpos() == 500)
                {
                    arrow->setXpos(60);
                    arrow->setYpos(260);
                    break;
                }
                if(keystate[SDL_SCANCODE_RETURN] && arrow->getYpos() == 500)
                {
                    clienting();
                    break;
                }
            }
        
        case SDL_TEXTINPUT:
            SDL_StartTextInput();
            connectingIp += event.text.text;
            ipValue->setText(connectingIp, black);
//            system("clear");
            std::cout << connectingIp << std::endl;
            break;
        default:
            break;
    }
}

void MultiplaySettingScreen::update()
{
    screen->update();
    host->update();
    guest->update();
    ipInput->update();
    ipValue->update();
    portInput->update();
    portValue->update();
    arrow->update();
    
}

void MultiplaySettingScreen::render()
{
    SDL_RenderClear(rend);
    
    // this is where we put things to render
    screen->render();
    
    if(!isHost && !isGuest)
    {
        host->render();
        guest->render();
        arrow->render();
    }
    
    if(isHost)
    {
        ipInput->render();
        ipValue->render();
        portInput->render();
        portValue->render();
        waiting->render();
    }
    if(isGuest)
    {
        ipInput->render();
        ipValue->render();
        portInput->render();
        portValue->render();
        connect->render();
        arrow->render();
    }
    
    SDL_RenderPresent(rend);
    if(isHost)
        hosting();
    if(isGuest)
        clienting();
}

TCPsocket MultiplaySettingScreen::getServer()
{
    return server;
}

TCPsocket MultiplaySettingScreen::getClient()
{
    return client;
}

void MultiplaySettingScreen::hosting()
{
    if (SDLNet_Init() == -1)
        std::cout << "SDLNET init failed" << std::endl;
    
    IPaddress ip;
    SDLNet_ResolveHost(&ip, NULL, 80);
    server = SDLNet_TCP_Open(&ip);
    
    // waiting for any client...
    SDL_Event event;
    while(SDL_WaitEvent(&event))
    {
        client = SDLNet_TCP_Accept(server);
        if(client)
        {
            SDLNet_TCP_Send(client, "Hello", 6);
            SDLNet_TCP_Close(client);
            break;
        }
    }
}

void MultiplaySettingScreen::clienting()
{
    std::cout << "connect!" << std::endl;
    if (SDLNet_Init() == -1)
        std::cout << "SDLNET init failed" << std::endl;

    IPaddress ip;
    SDLNet_ResolveHost(&ip, connectingIp.c_str(), atoi(connectingPort.c_str()));
    client = SDLNet_TCP_Open(&ip);
    char text[100];
    SDLNet_TCP_Recv(client, text, 100);
    std::cout << text;
}


