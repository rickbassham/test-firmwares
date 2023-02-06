#pragma once
#ifndef DragonLightServer_h
#define DragonLightServer_h

#include <Arduino.h>
#include <DeviceServer.h>

namespace DarkDragons
{
class DragonLightServer : public DeviceServer
{
public:
    DragonLightServer() : DeviceServer()
    {
    }

    static String boardName()
    {
        return "DragonLight";
    }

    void onRoot();
    void handleAlpacaManagementConfiguredDevices() override;
    void handleAlpacaGetDescription() override;
    void handleAlpacaGetName() override;

    void registerHandlers(EthernetWebServer &server) override
    {
        DeviceServer::registerHandlers(server);

        server.on("/", [&]() {
            onRoot();
        });
    }


private:
    bool isLightOn = false;
    byte lightLevel = 0;
    float temperature = 0.0;
    float humidity = 0.0;
    float dewPoint = 0.0;


}; // class DragonLightServer
}; // namespace DarkDragons

#endif
