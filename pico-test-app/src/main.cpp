#include "config.h"

#include <Arduino.h>
#include <Bounce2.h>

#include <SafetySwitch.h>
#include <Buzzer.h>
#include <HandController.h>
#include <Motor.h>

using namespace DarkDragons;

Motor motor;
HandController handController;

SafetySwitch safeToMove1Button;
SafetySwitch safeToMove2Button;

Bounce2::Button closeLimit;
Bounce2::Button openLimit;

Buzzer buzzer;

void onOpenPressed()
{
    if (!safeToMove1Button.isSafe() || !safeToMove2Button.isSafe())
    {
        Serial.println("Not safe to move");
        buzzer.beep(100, 2);
        return;
    }

    motor.setDesiredState(Motor::State::RunningForward);
    Serial.println("Open button pressed");
}

void onOpenReleased()
{
    motor.setDesiredState(Motor::State::Idle);
    Serial.println("Open button released");
}

void onClosePressed()
{
    if (!safeToMove1Button.isSafe() || !safeToMove2Button.isSafe())
    {
        Serial.println("Not safe to move");
        buzzer.beep(100, 2);
        return;
    }

    motor.setDesiredState(Motor::State::RunningBackward);
    Serial.println("Close button pressed");
}

void onCloseReleased()
{
    motor.setDesiredState(Motor::State::Idle);
    Serial.println("Close button released");
}

void onAbortPressed()
{
    motor.setDesiredState(Motor::State::Idle);
    Serial.println("Abort button pressed");
}

void onToggleOpenPressed()
{
    if (!safeToMove1Button.isSafe() || !safeToMove2Button.isSafe())
    {
        Serial.println("Not safe to move");
        buzzer.beep(100, 2);
        return;
    }

    motor.setDesiredState(Motor::State::RunningForward);
    Serial.println("Toggle open button pressed");
}

void onToggleClosePressed()
{
    if (!safeToMove1Button.isSafe() || !safeToMove2Button.isSafe())
    {
        Serial.println("Not safe to move");
        buzzer.beep(100, 2);
        return;
    }

    motor.setDesiredState(Motor::State::RunningBackward);
    Serial.println("Toggle close button pressed");
}

void setup()
{
    sleep_ms(4000);

    Serial.begin(9600);

    analogWriteResolution(8);
    analogWriteFreq(18000);

    analogReadResolution(8);

    // MOTOR CONTROL
    motor.attach(FORWARD_MOTOR, BACKWARD_MOTOR, ON_A, ON_B, CURRENT_SENSE);
    motor.setMaxSpeed(255);

    // INPUT BUTTONS
    handController.attach(OPEN, CLOSE, ABORT, TOGGLE_OPEN, TOGGLE_CLOSE);
    handController.onOpenPressed(onOpenPressed);
    handController.onOpenReleased(onOpenReleased);
    handController.onClosePressed(onClosePressed);
    handController.onCloseReleased(onCloseReleased);
    handController.onAbortPressed(onAbortPressed);
    handController.onToggleOpenPressed(onToggleOpenPressed);
    handController.onToggleClosePressed(onToggleClosePressed);

    // LIMIT SWITCHES
    closeLimit.attach(CLOSE_LIMIT, INPUT_PULLUP);
    openLimit.attach(OPEN_LIMIT, INPUT_PULLUP);

    // SAFETY_SENSORS
    safeToMove1Button.attach(SAFE_TO_MOVE_1);
    safeToMove2Button.attach(SAFE_TO_MOVE_2);

    // INPUTS
    pinMode(MAX_SPEED, INPUT);

    // OUTPUTS
    buzzer.attach(BUZZER);

    // buzzer.beep(100, 2);

    safeToMove1Button.update();
    safeToMove2Button.update();

    if (safeToMove1Button.isSafe())
    {
        Serial.println("Safe to move 1");
    }

    if (safeToMove2Button.isSafe())
    {
        Serial.println("Safe to move 2");
    }
}

void loop()
{
    sleep_ms(100);

    closeLimit.update();
    openLimit.update();

    if (closeLimit.fell())
    {
        motor.setDesiredState(Motor::State::Idle);
        Serial.println("Close limit switch");
    }

    if (openLimit.fell())
    {
        motor.setDesiredState(Motor::State::Idle);
        Serial.println("Open limit switch");
    }

    safeToMove1Button.update();
    safeToMove2Button.update();
    handController.update();

    motor.run();

    if (safeToMove1Button.changed())
    {
        if (safeToMove1Button.isSafe())
        {
            Serial.println("Safe to move 1 is safe to move");
        }
        else
        {
            Serial.println("Safe to move 1 is NOT safe to move");

            if (motor.isMoving())
            {
                motor.setDesiredState(Motor::State::Idle);
            }
        }
    }

    if (safeToMove2Button.changed())
    {
        if (safeToMove2Button.isSafe())
        {
            Serial.println("Safe to move 2 is safe to move");
        }
        else
        {
            Serial.println("Safe to move 2 is NOT safe to move");

            if (motor.isMoving())
            {
                motor.setDesiredState(Motor::State::Idle);
            }
        }
    }

    float current = motor.readCurrent();
    Serial.print(current);
    Serial.println(" A");
}
