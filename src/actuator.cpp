#include "..\include\actuator.h"

Actuator::Actuator(unsigned char pin, uint16_t offTime, uint16_t onTime)
    : pin(pin), offTime(offTime), onTime(onTime), mode(OFF), pinState(false), lastTransition(millis())
{
    pinMode(pin, OUTPUT);
    setPinState(false);
}

void Actuator::setOn()
{
    mode = ON;
    setPinState(true);
}

void Actuator::setOff()
{
    mode = OFF;
    setPinState(false);
}

void Actuator::setBlinking()
{
    mode = BLINKING;
    setPinState(true);
}

void Actuator::setTiming(uint16_t offTime, uint16_t onTime)
{
    this->offTime = offTime;
    this->onTime = onTime;
}

void Actuator::update()
{
    if (mode != BLINKING)
    {
        return;
    }

    uint32_t now = millis();
    uint32_t elapsed = now - lastTransition;

    if (pinState)
    {
        if (elapsed >= onTime)
        {
            setPinState(false);
        }
    }
    else
    {
        if (elapsed >= offTime)
        {
            setPinState(true);
        }
    }
}

bool Actuator::isOn() const
{
    return pinState;
}

bool Actuator::isBlinking() const
{
    return mode == BLINKING;
}

void Actuator::setPinState(bool state)
{
    pinState = state;
    digitalWrite(pin, state ? HIGH : LOW);
    lastTransition = millis();
}

Actuator sireneActuator(6, 500, 500);
Actuator lampActuator(13, 500, 500);
