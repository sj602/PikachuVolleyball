#pragma once
#include "Screen.h"

class MultiplaySettingScreen: public Screen
{
private:
    std::string connectingIp = "127.0.0.1";
    std::string connectingPort = "80";
public:
    MultiplaySettingScreen();
    ~MultiplaySettingScreen();
    virtual void handleEvents(const Uint8 *keystate, bool *isSelecting, bool *isSingle, bool *isMulti) override;
    virtual void Update() override;
    virtual void Render() override;
};
