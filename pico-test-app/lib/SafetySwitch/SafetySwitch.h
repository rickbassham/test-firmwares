#ifndef SafetySwitch_h
#define SafetySwitch_h

#include <Arduino.h>
#include <inttypes.h>

#include <Bounce2.h>

namespace DarkDragons {
class SafetySwitch {
public:
    SafetySwitch() = default;

    bool isSafe()
    {
        return button.read() == HIGH;
    }

    void update()
    {
        button.update();
    }

    void attach(uint8_t pin)
    {
        button.attach(pin, INPUT);
    }

    bool changed()
    {
        return button.changed();
    }

private:
    Bounce2::Button button;

}; // class SafetySwitch
}; // namespace DarkDragons

#endif
