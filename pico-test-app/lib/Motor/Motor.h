#ifndef Motor_h
#define Motor_h

#include <Arduino.h>
#include <inttypes.h>

namespace DarkDragons {
class Motor {
public:
    enum State {
        Idle,
        AcceleratingForward,
        AcceleratingBackward,
        DeceleratingForward,
        DeceleratingBackward,
        RunningForward,
        RunningBackward,
    };

public:
    Motor() = default;

    void attach(uint8_t forwardMotorPin, uint8_t backwardMotorPin, uint8_t onAPin, uint8_t onBPin, uint8_t currentSensePin);
    void run();

    State state() const
    {
        return _state;
    }

    void setDesiredState(State state)
    {
        _desiredState = state;
    }

    uint8_t currentSpeed() const
    {
        return _currentSpeed;
    }

    void setMinSpeed(uint8_t minSpeed)
    {
        _minSpeed = minSpeed;
    }

    void setMaxSpeed(uint8_t maxSpeed)
    {
        _maxSpeed = maxSpeed;
    }

    void enable(bool enable)
    {
        digitalWrite(_onAPin, enable ? HIGH : LOW);
        digitalWrite(_onBPin, enable ? HIGH : LOW);
    }

    bool isMoving() const
    {
        return _state != Idle;
    }

    float readCurrent() const
    {
        return analogRead(_currentSensePin) / 255.0 * 3.3;
    }

private:
    uint8_t calculateSpeed();

    void handleDesiredIdleState();
    void handleDesiredRunningForwardState();
    void handleDesiredRunningBackwardState();

private:
    uint8_t _forwardMotorPin;
    uint8_t _backwardMotorPin;
    uint8_t _currentSensePin;
    uint8_t _onAPin;
    uint8_t _onBPin;

    State _state = Idle;
    State _desiredState = Idle;

    uint8_t _currentSpeed;

    uint8_t _minSpeed = 0;
    uint8_t _maxSpeed = 255;

    uint8_t _startSpeed;
    unsigned long _lastStartTime;

}; // class Motor
}; // namespace DarkDragons

#endif
