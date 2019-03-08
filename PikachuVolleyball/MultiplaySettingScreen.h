#pragma once
#include <vector>
#include "Player.h"
#include "Screen.h"

class MultiplaySettingScreen: public Screen
{
private:
    std::string connectingIp = "127.0.0.1";
    std::string connectingPort = "80";
    
    bool isInitial = true;
    bool isWaiting;
    
    SDLNet_SocketSet serverSocketSet;
    SDLNet_SocketSet clientSocketSet;
    std::vector<TCPsocket> serverSocketList;
    std::vector<TCPsocket> clientSocketList;

public:
    explicit MultiplaySettingScreen(SDL_Renderer* _renderer);
    ~MultiplaySettingScreen();
    void handleEvents(const Uint8*& keystate, bool& isSelecting, bool& isSingle, bool& isMulti, bool& isHost, bool& isGuest, bool& isMultiConnected, Player* p, TCPsocket& client);
    virtual void update() override;
    void render(bool& isMultiConnected, bool& isHost, bool& isGuest, Player* p, TCPsocket& server, TCPsocket& client);
    TCPsocket& getServer();
    TCPsocket& getClient();
    SDLNet_SocketSet& getServerSocketSet();
    void hosting(bool& isMultiConnected, Player* p, TCPsocket& server, TCPsocket& client);
    void clienting(bool& isMultiConnected, Player* p, TCPsocket& client);
};
