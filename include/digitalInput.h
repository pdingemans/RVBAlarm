#pragma once
#include "input.h"

class DigitalInput : public Input
{
    using Input::Input;

public:
    void init() override
    {
        pinMode(getPin(), INPUT_PULLUP);
    }

private:
    inline uint16_t readInput(uint8_t pinnr)
    {
        return !digitalRead(pinnr);
    }
};