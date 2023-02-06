#include "Motor.h"

#define ACCELERATION_TIME 5000
#define DECELERATION_TIME 250

using namespace DarkDragons;

void Motor::attach(uint8_t forwardMotorPin, uint8_t backwardMotorPin, uint8_t onAPin, uint8_t onBPin, uint8_t currentSensePin)
{
    _currentSpeed = 0;

    _forwardMotorPin = forwardMotorPin;
    _backwardMotorPin = backwardMotorPin;
    _currentSensePin = currentSensePin;
    _onAPin = onAPin;
    _onBPin = onBPin;

    pinMode(_forwardMotorPin, OUTPUT);
    pinMode(_backwardMotorPin, OUTPUT);
    pinMode(_onAPin, OUTPUT);
    pinMode(_onBPin, OUTPUT);
    pinMode(_currentSensePin, INPUT);

    enable(true);
}

uint8_t Motor::calculateSpeed()
{
    switch (_state) {
    case Idle:
        return 0;
    case AcceleratingForward:
    case AcceleratingBackward:
        return min(map(millis() - _lastStartTime, 0, ACCELERATION_TIME, _startSpeed, _maxSpeed), _maxSpeed);
        break;
    case DeceleratingForward:
    case DeceleratingBackward:
        return _startSpeed - min(map(millis() - _lastStartTime, 0, DECELERATION_TIME, 0, _startSpeed), _startSpeed);
        break;
    case RunningForward:
    case RunningBackward:
        return _currentSpeed;
        break;
    }

    return 0;
}

// Desired state is Idle, which means we need to work towards being stopped.
// If we are accelerating, we need to start decelerating, then stop once we reach 0 speed.
// If we are decelerating, we need to continue decelerating until we reach 0 speed.
void Motor::handleDesiredIdleState()
{
    switch (_state)
    {
    case Idle:
        // no-op; current state is desired state
        break;
    case AcceleratingForward:
        _state = DeceleratingForward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case AcceleratingBackward:
        _state = DeceleratingBackward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case DeceleratingForward:
        _currentSpeed = calculateSpeed();
        analogWrite(_forwardMotorPin, _currentSpeed);
        analogWrite(_backwardMotorPin, 0);

        if (_currentSpeed == 0) {
            _state = Idle;
        }
        break;
    case DeceleratingBackward:
        _currentSpeed = calculateSpeed();
        analogWrite(_backwardMotorPin, _currentSpeed);
        analogWrite(_forwardMotorPin, 0);

        if (_currentSpeed == 0) {
            _state = Idle;
        }
        break;
    case RunningForward:
        _state = DeceleratingForward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case RunningBackward:
        _state = DeceleratingBackward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    }
}

// Desired state is RunningForward, which means we need to work towards being running forward.
// If we are idle, we need to start accelerating.
// If we are accelerating, we need to continue accelerating until we reach max speed.
// If we are decelerating, we need to start accelerating.
// If we are running backward, we need to start decelerating, then switch directions once we reach 0 speed.
void Motor::handleDesiredRunningForwardState()
{
    switch (_state) {
    case Idle:
        _state = AcceleratingForward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case AcceleratingForward:
        _currentSpeed = calculateSpeed();
        analogWrite(_forwardMotorPin, _currentSpeed);
        analogWrite(_backwardMotorPin, 0);

        if (_currentSpeed == _maxSpeed) {
            _state = RunningForward;
        }
        break;
    case AcceleratingBackward:
        _state = DeceleratingBackward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case DeceleratingForward:
        _state = AcceleratingForward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case DeceleratingBackward:
        _currentSpeed = calculateSpeed();
        analogWrite(_backwardMotorPin, _currentSpeed);
        analogWrite(_forwardMotorPin, 0);

        if (_currentSpeed == 0) {
            _state = AcceleratingForward;
            _lastStartTime = millis();
            _startSpeed = _currentSpeed;
        }
        break;
    case RunningForward:
        // no-op; current state is desired state
        break;
    case RunningBackward:
        _state = DeceleratingBackward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    }
}

void Motor::handleDesiredRunningBackwardState()
{
    switch (_state) {
    case Idle:
        _state = AcceleratingBackward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case AcceleratingForward:
        _state = DeceleratingForward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case AcceleratingBackward:
        _currentSpeed = calculateSpeed();
        analogWrite(_backwardMotorPin, _currentSpeed);
        analogWrite(_forwardMotorPin, 0);

        if (_currentSpeed == _maxSpeed) {
            _state = RunningBackward;
        }
        break;
    case DeceleratingForward:
        _currentSpeed = calculateSpeed();
        analogWrite(_forwardMotorPin, _currentSpeed);
        analogWrite(_backwardMotorPin, 0);

        if (_currentSpeed == 0) {
            _state = AcceleratingBackward;
            _lastStartTime = millis();
            _startSpeed = _currentSpeed;
        }
        break;
    case DeceleratingBackward:
        _state = AcceleratingBackward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case RunningForward:
        _state = DeceleratingForward;
        _lastStartTime = millis();
        _startSpeed = _currentSpeed;
        break;
    case RunningBackward:
        // no-op; current state is desired state
        break;
    }
}

void Motor::run()
{
    switch (_desiredState) {
    case Idle:
        handleDesiredIdleState();
        break;
    case AcceleratingForward:
        _desiredState = RunningForward;
        break;
    case AcceleratingBackward:
        _desiredState = RunningBackward;
        break;
    case DeceleratingForward:
        _desiredState = Idle;
        break;
    case DeceleratingBackward:
        _desiredState = Idle;
        break;
    case RunningForward:
        handleDesiredRunningForwardState();
        break;
    case RunningBackward:
        handleDesiredRunningBackwardState();
        break;
    }

#ifdef DEBUG
    Serial.print("State: ");
    Serial.print(_state);
    Serial.print(" Desired State: ");
    Serial.print(_desiredState);
    Serial.print(" Current Speed: ");
    Serial.print(_currentSpeed);
    Serial.print(" Start Speed: ");
    Serial.print(_startSpeed);
    Serial.print(" Last Start Time: ");
    Serial.print(_lastStartTime);
    Serial.print(" Current Time: ");
    Serial.print(millis());
    Serial.print(" Max Speed: ");
    Serial.println(_maxSpeed);
#endif
}
