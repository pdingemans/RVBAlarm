#pragma once
#include "input.h"

class AnalogInput : public Input
{
       //Input(uint8_t pinnr, uint16_t threshold,uint16_t poltime,uint8_t counts,uint16_t inactiveTime):
    using Input::Input;
    private:
    inline  uint16_t readInput(uint8_t pinnr)
    {
        return analogRead(pinnr);
    }
     };