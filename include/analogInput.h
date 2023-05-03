#pragma once
#include "input.h"

class AnalogInput : public Input
{
    using Input::Input;
    private:
    inline  uint16_t readInput(uint8_t pinnr)
    {
        return analogRead(pinnr);
    }
     };