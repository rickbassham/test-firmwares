#include "RP2040Device.h"

using namespace DarkDragons;

RP2040Device::RP2040Device(uint8_t boardId, String boardName)
{
    _boardId = boardId;
    _boardName = boardName;
}

String RP2040Device::boardname()
{
    return _boardName;
}

uint8_t *RP2040Device::macAddress()
{
    if (_macAddress.length() > 0)
    {
        return _mac;
    }

    _mac = (uint8_t*)malloc(6);
    memset(_mac, 0, 6);

    pico_unique_board_id_t id;
    pico_get_unique_board_id(&id);

    _mac[0] = 0x02; // Self Defined Mac
    _mac[1] = _boardId;
    _mac[2] = id.id[4];
    _mac[3] = id.id[5];
    _mac[4] = id.id[6];
    _mac[5] = id.id[7];

    char macStr[18];
    memset(&macStr, 0, 18);

    sprintf(
        macStr,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]
    );

    _macAddress = String(macStr);

    return _mac;
}

String RP2040Device::serial()
{
    if (_serial.length() > 0)
    {
        return _serial;
    }

    char buffer[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
    pico_get_unique_board_id_string(buffer, sizeof(buffer));

    _serial = String(buffer);

    return _serial;
}

String RP2040Device::hostname()
{
    if (_hostname.length() > 0)
    {
        return _hostname;
    }

    _hostname = _boardName + "_" + serial();

    _hostname.toLowerCase();

    return _hostname;
}
