#pragma once
#include "input.h"

class DigitalInput : public Input
{
    using Input::Input;

private:
  

    inline uint8_t readInput(uint8_t pinnr)
    {
        pinMode(pinnr,INPUT_PULLUP);
        return !digitalRead(pinnr);
    }
};