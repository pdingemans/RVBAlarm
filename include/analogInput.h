#pragma once
#include "input.h"

class AnalogInput : public Input
{
    using Input::Input;
    private:

    inline  uint8_t readInput(uint8_t pinnr)
    {
        uint16_t val =  analogRead(pinnr);
        return val>=getThreshold() ? HIGH:LOW; 
        return val;
    }
     };