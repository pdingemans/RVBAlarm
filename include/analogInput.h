#pragma once
#include "input.h"
constexpr uint8_t len = 10;
class AnalogInput : public Input
{
    using Input::Input;

private:
    uint16_t samples[len];
    uint8_t idx;
    uint32_t total;
    inline void init()
    {
        for (size_t i = 0; i < len; i++)
        {
            samples[i] = 0;
        }
        idx = 0;
    }
    inline uint16_t readInput(uint8_t pinnr)
    {
        uint16_t val = 0;

        samples[idx] = analogRead(pinnr);
        // Serial.print("pin");
        // Serial.print(pinnr);
        // Serial.print(":");
        // Serial.print("idx");
        // Serial.print(idx);
        // Serial.print(":");
        // Serial.print(samples[idx]);
        // Serial.print(":");

        idx++;
        // not really a moving average, but we take len samples and take the average
        // should be ok for now (I hope)
        total = 0;
        for (size_t i = 0; i < len; i++)
        {
            total += samples[i];
        }

        idx %= len; // wrap around

        val = total / len; // calculate the average
        // Serial.print(total);
        // Serial.print(":");

        // Serial.println(val);
        //  Serial.print("---");
        return val; //>= getThreshold() ? HIGH : LOW;
    }
};