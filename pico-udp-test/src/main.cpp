#define DEBUG
#define ETHERNET_LARGE_BUFFERS
#define _ETHERNET_WEBSERVER_LOGLEVEL_ 4
#define HTTP_UPLOAD_BUFLEN 2048

#include <Arduino.h>

extern "C"
{
#include "libb64/cencode.h"
}

#include <SPI.h>
#include <Ethernet_Generic.h>
#include <MDNS_Generic.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <EthernetWebServer.h>

#include <EthernetHttpClient.h>

#include <Updater.h>

extern "C"
{
#include <hardware/watchdog.h>
};

#include "unzip_helpers.h"
#include "mac_helper.h"
#include "mimetype.h"

#define USE_THIS_SS_PIN 17

#define RESPONSE_BUFFER_SIZE 512

EthernetWebServer server(80);

EthernetUDP udp;
MDNS mdns(udp);

byte *mac = generateMacAddress();
MD5Builder md5;

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

void handle()
{
    server.method() == HTTP_GET ? Serial.println("GET") : Serial.println("POST");

    server.send(200, "text/plain", (std::string("Hello, world!\n") + std::to_string(millis()) + std::string("\n")).c_str());
}

void setup()
{
    sleep_ms(5000);

    Serial.begin(9600);

    String hostname = generateHostname();

#ifdef DEBUG
    Serial.println(getMac());
#endif

    Ethernet.setHostname(hostname.c_str());
    Ethernet.init(USE_THIS_SS_PIN);
    if (Ethernet.begin(mac, 10000, 4000) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        blinkErrorCode(5);
    }

#ifdef DEBUG
    Serial.print(F("Connected! IP address: "));
    Serial.println(Ethernet.localIP());
#endif

    if (!LittleFS.begin())
    {
        Serial.println("Failed to initialize file system");
        blinkErrorCode(3);
    }

    mdns.begin(Ethernet.localIP(), hostname.c_str());
    mdns.addServiceRecord("DragonLair._http",
                          80,
                          MDNSServiceTCP);

    const char *headerkeys[] = {"If-None-Match"};
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);

    // ask server to track these headers
    server.collectHeaders(headerkeys, headerkeyssize);
    server.onNotFound(handle);

    server.begin();

#ifdef DEBUG
    Serial.println(getBoardSerial());
    Serial.println(hostname);
#endif

    watchdog_enable(0x7fffff, false);
}

void loop()
{
    watchdog_update();

    Ethernet.maintain();
    mdns.run();
    server.handleClient();

    delay(1);
}