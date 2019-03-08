#include <regex>
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

std::regex ipReg("[0-9.]");
std::regex portReg("[0-9]");

MultiplaySettingScreen::MultiplaySettingScreen(SDL_Renderer* _renderer): Screen(_renderer)
{
    std::cout << "MultiplaySettingScreen constructor()!" << std::endl;

    host = new GameObject("Host mode", MODE_WIDTH, MODE_HEIGHT, 300, 330, white);
    guest = new GameObject("Guest mode", MODE_WIDTH, MODE_HEIGHT, 300, 390, white);
    ipInput = new GameObject("Connecting IP: ", MODE_WIDTH, MODE_HEIGHT, 100, 200, white);
    ipValue = new GameObject(connectingIp, TEXT_WIDTH, TEXT_HEIGHT, 300, 200, black);
    portInput = new GameObject("Connecting PORT: ", MODE_WIDTH, MODE_HEIGHT, 100, 250, white);
    portValue = new GameObject(connectingPort, MODE_WIDTH, MODE_HEIGHT, 300, 250, black);
    waiting = new GameObject("Waiting...", MODE_WIDTH, MODE_HEIGHT, 300, 500, white);
    connect = new GameObject("Connect!", MODE_WIDTH, MODE_HEIGHT, 300, 500, white);
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

void MultiplaySettingScreen::handleEvents(const Uint8*& keystate, bool& isSelecting, bool& isSingle, bool& isMulti, bool& isHost, bool& isGuest, bool& isMultiConnected, Player* p, TCPsocket& client)
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
                if(isInitial)
                {
                    arrow->setXpos(60);
                    arrow->setYpos(210);
                    isInitial = false;
                }
                if(keystate[SDL_SCANCODE_BACKSPACE] && arrow->getYpos() == 210 && connectingIp.length() > 0)
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
                if(keystate[SDL_SCANCODE_UP] && arrow->getYpos() == 510)
                {
                    arrow->setXpos(60);
                    arrow->setYpos(260);
                    break;
                }
                if(keystate[SDL_SCANCODE_RETURN] && arrow->getYpos() == 510)
                {
                    clienting(isMultiConnected, p, client);
                    break;
                }
            }
        
        case SDL_TEXTINPUT:
            SDL_StartTextInput();

            if(arrow->getYpos() == 210 && connectingIp.length() <= 15 && std::regex_match(event.text.text, ipReg))
            {
                connectingIp += event.text.text;
                ipValue->setText(connectingIp, black);
            }
            if(arrow->getYpos() == 260 && connectingPort.length() <= 2 && std::regex_match(event.text.text, portReg))
            {
                connectingPort += event.text.text;
                portValue->setText(connectingPort, black);
            }
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

void MultiplaySettingScreen::render(bool& isMultiConnected, bool& isHost, bool& isGuest, Player* p, TCPsocket& server, TCPsocket& client)
{
    SDL_RenderClear(renderer);
    
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
    
    SDL_RenderPresent(renderer);
    if(isHost)
        hosting(isMultiConnected, p, server, client);
}

SDLNet_SocketSet& MultiplaySettingScreen::getServerSocketSet()
{
    return serverSocketSet;
}

void MultiplaySettingScreen::hosting(bool& isMultiConnected, Player* p, TCPsocket& server, TCPsocket& client)
{
    if (SDLNet_Init() == -1)
        std::cout << "SDLNET init failed" << std::endl;
    
    IPaddress ip;
    
    if(SDLNet_ResolveHost(&ip, NULL, atoi(connectingPort.c_str())) == -1)
        std::cout << "Failed to open port: " << connectingPort << std::endl;
    
    server = SDLNet_TCP_Open(&ip);
    if(server == NULL)
        std::cout << "Failed to open port for listening: " << SDL_GetError() << std::endl;
    
    serverSocketSet = SDLNet_AllocSocketSet(2);
    if(serverSocketSet == NULL)
        std::cout << "Failed to open socket set: " << SDL_GetError() << std::endl;

    SDLNet_TCP_AddSocket(serverSocketSet, server);

    // waiting for any client...
    SDL_Event event;
    while(SDL_WaitEvent(&event))
    {
        client = SDLNet_TCP_Accept(server);
        if(client)
        {
            isMultiConnected = true;
            p->setFlag('R');
            p->setxVel(0);

            SDLNet_TCP_Send(client, "Hello", 6);
            break;
        }
    }
}

void MultiplaySettingScreen::clienting(bool& isMultiConnected, Player* p, TCPsocket& client)
{
    if (SDLNet_Init() == -1)
        std::cout << "SDLNET init failed" << std::endl;

    IPaddress ip;
    if(SDLNet_ResolveHost(&ip, connectingIp.c_str(), atoi(connectingPort.c_str())) == -1)
    {
        std::cout << "Failed to open port: " << connectingPort << std::endl;
    }
    
    client = SDLNet_TCP_Open(&ip);
    if(!client)
    {
        std::cout << "Failed to connect to port: " << connectingPort << " and : " << SDL_GetError() << std::endl;
        return;
    }
    else
    {
        isMultiConnected = true;
        p->setFlag('R');
        p->setxVel(0);

        char text[100];
        int num_recv = SDLNet_TCP_Recv(client, text, 100);
        
        if(num_recv <= 0)
            std::cout << "Received data failed: " << SDL_GetError() << std::endl;
        else
            std::cout << text << std::endl;
        
    }
}

