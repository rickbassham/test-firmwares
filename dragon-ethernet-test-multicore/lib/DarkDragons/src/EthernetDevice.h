#pragma once
#ifndef EthernetDevice_h
#define EthernetDevice_h

#include "RP2040Device.h"
#include "DeviceServer.h"

#include <Ethernet_Generic.hpp>
#include <EthernetWebServer.hpp>
#include <MDNS_Generic.hpp>

namespace DarkDragons
{
template <class TDeviceServer>
class EthernetDevice : public RP2040Device
{
public:
    EthernetDevice(uint8_t boardId, String boardName): RP2040Device(boardId, boardName), mdns(udp), server(80)
    {
    }

    bool setup()
    {
        Ethernet.setHostname(hostname().c_str());
        Ethernet.init(17);

        if (Ethernet.begin(macAddress(), 10000, 4000) == 0)
        {
            Serial.println("Failed to configure Ethernet using DHCP");
            return false;
        }

        mdns.begin(Ethernet.localIP(), hostname().c_str());
        mdns.addServiceRecord((boardname() + "._http").c_str(),
                              80,
                              MDNSServiceTCP);

        _device.setHostname(hostname());
        _device.registerHandlers(server);

        server.onNotFound([&]() {
            server.send(404, "text/plain", "Not found");
        });

        server.begin();

        return true;
    }

    void run()
    {
        Ethernet.maintain();
        server.handleClient();
        mdns.run();
    }

protected:
    EthernetWebServer server;
    EthernetUDP udp;
    MDNS mdns;

private:
    TDeviceServer _device;

}; // class EthernetDevice
}; // namespace DarkDragons

#endif
