#pragma once
#ifndef RP2040Device_h
#define RP2040Device_h

#include <Arduino.h>
#include <pico/unique_id.h>

namespace DarkDragons
{
class RP2040Device
{
public:
    RP2040Device(uint8_t boardId, String boardName);
    uint8_t *macAddress();
    String serial();
    String hostname();
    String boardname();

private:
    uint8_t _boardId;
    String _hostname;
    String _macAddress;
    uint8_t *_mac;
    String _serial;
    String _boardName;

}; // class RP2040Device
}; // namespace DarkDragons

#endif
