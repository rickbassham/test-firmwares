#ifndef Buzzer_h
#define Buzzer_h

#include <Arduino.h>
#include <inttypes.h>

namespace DarkDragons {
class Buzzer {
public:
    Buzzer() = default;

    void attach(uint8_t pin)
    {
        pinMode(pin, OUTPUT);
        _pin = pin;
    }

    void beep(uint32_t delay, int times)
    {
        for (int i = 0; i < times; i++) {
            digitalWrite(_pin, HIGH);
            sleep_ms(delay);
            digitalWrite(_pin, LOW);
            sleep_ms(delay);
        }
    }

private:
    uint8_t _pin;

}; // class Buzzer
}; // namespace DarkDragons

#endif
