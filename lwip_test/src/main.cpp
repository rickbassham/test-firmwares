#define USE_THIS_SS_PIN 17

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet_Generic.h>
#include <LittleFS.h>

#include "ethernet_server.h"
#include "mac_helper.h"

uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

void handleRequest(EthernetClient &client, const request_t req)
{
    response_t resp{
        req.version,
        200,
        "OK",
        2,
        "close",
        "text/plain",
        "",
        ""};

    sendHeaders(&client, resp);
    client.print("OK");
}

void blinkErrorCode(int blinks)
{
    while (true)
    {
        for (int i = 0; i < blinks; i++)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            sleep_ms(200);
            digitalWrite(LED_BUILTIN, LOW);
            sleep_ms(200);
        }

        sleep_ms(5000);
    }
}

void setup()
{
    sleep_ms(5000);

    Serial.begin(9600);

    String hostname = generateHostname();

    Ethernet.setHostname(hostname.c_str());
    Ethernet.init(USE_THIS_SS_PIN);
    if (Ethernet.begin(mac, 10000, 4000) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        blinkErrorCode(5);
    }

    Serial.print(F("Connected! IP address: "));
    Serial.println(Ethernet.localIP());

    if (!LittleFS.begin())
    {
        Serial.println("Failed to initialize file system");
        blinkErrorCode(3);
    }

    server.begin();

    Serial.println(getBoardSerial());
    Serial.println(hostname);
}

void loop()
{
    Ethernet.maintain();
    handleClient(handleRequest);
}