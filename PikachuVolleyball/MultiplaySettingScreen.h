#pragma once
#include "Screen.h"

class MultiplaySettingScreen: public Screen
{
private:
    std::string connectingIp = "";
    std::string connectingPort = "80";
    bool isHost = false;
    bool isGuest = false;
    bool isWaiting = false;
    
    TCPsocket server = NULL;
    TCPsocket client = NULL;
public:
    explicit MultiplaySettingScreen(SDL_Renderer *renderer);
    ~MultiplaySettingScreen();
    virtual void handleEvents(const Uint8 *keystate, bool &isSelecting, bool &isSingle, bool &isMulti) override;
    virtual void update() override;
    virtual void render() override;
    TCPsocket getServer();
    TCPsocket getClient();
    void hosting();
    void clienting();
};
