#include <Arduino.h>
#include <Ethernet_Generic.h>
#include <EthernetWebServer.h>
#include <MDNS_Generic.h>

#include "DragonLightServer.h"
#include <EthernetDevice.h>

DarkDragons::EthernetDevice<DarkDragons::DragonLightServer> device(0xDA);

void setup()
{
    delay(5000);

    // put your setup code here, to run once:
    Serial.begin(9600);

    device.setup();

    Serial.println("Ethernet started");
    Serial.print("IP address: ");
    Serial.println(Ethernet.localIP());
}

void loop()
{
    device.run();
}
