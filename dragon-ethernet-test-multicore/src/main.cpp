#include <Arduino.h>
#include <SPI.h>

#include <EthernetDevice.h>
#include <DragonLightServer.h>

#include <Ethernet_Generic.h>
#include <EthernetWebServer.h>
#include <MDNS_Generic.h>

#define BOARD_ID 0xDA

using namespace DarkDragons;

// Start Core 1 Variables and Functions

EthernetDevice<DragonLightServer> device(BOARD_ID, "DragonLight");

void setup1()
{
    sleep_ms(4000);

    device.setup();

    Serial.println("core1 ready");

    Serial.print("Hostname: ");
    Serial.println(device.hostname());

    rp2040.fifo.push(1);
}

void loop1()
{
    device.run();
}

// End Core 1 Variables and Functions

// Start Core 0 Variables and Functions

void setup()
{
    sleep_ms(4000);

    Serial.begin(9600);
    while (!Serial);

    Serial.println("core0 ready");
}

bool core1Running = false;

void loop()
{
    delay(1000);

    if (!core1Running)
    {
        uint32_t ready;

        if (rp2040.fifo.pop_nb(&ready))
        {
            Serial.println("core1 running");
            core1Running = true;
        }
    }
}

// End Core 0 Variables and Functions
