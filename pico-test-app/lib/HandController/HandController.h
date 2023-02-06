#ifndef HandController_h
#define HandController_h

#include <Arduino.h>
#include <inttypes.h>

#include <Bounce2.h>

namespace DarkDragons {
typedef void(*ButtonCallback)(void);

class HandController {
public:
    HandController() = default;

    void update()
    {
        openButton.update();
        closeButton.update();
        abortButton.update();
        toggleOpenButton.update();
        toggleCloseButton.update();

        if (openButton.fell() && _onOpenPressed) {
            _onOpenPressed();
        }

        if (openButton.rose() && _onOpenReleased) {
            _onOpenReleased();
        }

        if (closeButton.fell() && _onClosePressed) {
            _onClosePressed();
        }

        if (closeButton.rose() && _onCloseReleased) {
            _onCloseReleased();
        }

        if (abortButton.fell() && _onAbortPressed) {
            _onAbortPressed();
        }

        if (toggleOpenButton.fell() && _onToggleOpenPressed) {
            _onToggleOpenPressed();
        }

        if (toggleCloseButton.fell() && _onToggleClosePressed) {
            _onToggleClosePressed();
        }
    }

    void attach(uint8_t openPin, uint8_t closePin, uint8_t abortPin, uint8_t toggleOpenPin, uint8_t toggleClosePin)
    {
        openButton.attach(openPin, INPUT_PULLUP);
        closeButton.attach(closePin, INPUT_PULLUP);
        abortButton.attach(abortPin, INPUT_PULLUP);
        toggleOpenButton.attach(toggleOpenPin, INPUT_PULLUP);
        toggleCloseButton.attach(toggleClosePin, INPUT_PULLUP);
    }

    void onOpenPressed(ButtonCallback callback)
    {
        _onOpenPressed = callback;
    }

    void onOpenReleased(ButtonCallback callback)
    {
        _onOpenReleased = callback;
    }

    void onClosePressed(ButtonCallback callback)
    {
        _onClosePressed = callback;
    }

    void onCloseReleased(ButtonCallback callback)
    {
        _onCloseReleased = callback;
    }

    void onAbortPressed(ButtonCallback callback)
    {
        _onAbortPressed = callback;
    }

    void onToggleOpenPressed(ButtonCallback callback)
    {
        _onToggleOpenPressed = callback;
    }

    void onToggleClosePressed(ButtonCallback callback)
    {
        _onToggleClosePressed = callback;
    }

private:
    Bounce2::Button openButton;
    Bounce2::Button closeButton;
    Bounce2::Button abortButton;
    Bounce2::Button toggleOpenButton;
    Bounce2::Button toggleCloseButton;

    ButtonCallback _onOpenPressed;
    ButtonCallback _onOpenReleased;
    ButtonCallback _onClosePressed;
    ButtonCallback _onCloseReleased;
    ButtonCallback _onAbortPressed;

    ButtonCallback _onToggleOpenPressed;
    ButtonCallback _onToggleClosePressed;

}; // class HandController
}; // namespace DarkDragons

#endif
