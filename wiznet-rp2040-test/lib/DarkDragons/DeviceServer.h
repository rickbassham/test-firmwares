#pragma once
#ifndef DeviceServer_h
#define DeviceServer_h

#include <Arduino.h>
#include <EthernetWebServer.hpp>
#include <ArduinoJson.h>

#define RESPONSE_BUFFER_SIZE 512

namespace DarkDragons
{
class DeviceServer
{
public:
    DeviceServer()
    {

    }

    void sendDocument(StaticJsonDocument<RESPONSE_BUFFER_SIZE> &doc);
    void sendError(uint32_t clientTransactionId, int32_t errorNumber, String errorMessage);

    void handleNotImplemented();
    void handleAlpacaManagementApiVersions();
    void handleAlpacaManagementDescription();
    virtual void handleAlpacaManagementConfiguredDevices() = 0;
    void handleAlpacaGetConnected();
    void handleAlpacaPutConnected();
    virtual void handleAlpacaGetDescription() = 0;
    void handleAlpacaGetVersion();
    void handleAlpacaGetInterfaceVersion();
    virtual void handleAlpacaGetName() = 0;
    void handleAlpacaGetSupportedActions();

    void setHostname(String hostname)
    {
        this->hostname = hostname;
    }

    virtual void registerHandlers(EthernetWebServer &server);

protected:
    uint32_t transactionId = 1;
    String hostname;
    bool isConnected = false;
    EthernetWebServer* server;

private:

}; // class DeviceServer
}; // namespace DarkDragons

#endif
