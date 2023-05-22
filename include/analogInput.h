#pragma once
#include "input.h"
constexpr uint8_t len = 50;
class AnalogInput : public Input
{
    using Input::Input;

private:
    uint16_t samples[len];
    uint8_t idx = 0;
    uint32_t total;
    inline uint8_t readInput(uint8_t pinnr)
    {
        uint16_t val;
        samples[idx] = analogRead(pinnr);
        idx++;
        idx %= len;
        total = 0;
        for (size_t i = 0; i < len; i++)
        {
            total += samples[i];
        }
        val = total / len;
        Serial.println(val);
        return val >= getThreshold() ? HIGH : LOW;
    }
};