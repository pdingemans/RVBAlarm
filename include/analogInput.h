#pragma once
#include <Arduino.h>
#include <stdint.h>
#include "input.h"
constexpr uint8_t nrzerosamples = 100;
constexpr int32_t CUT_OFF_frequency = 10;
constexpr int32_t decayfactor = 50; // if we have lower value then our measurement, it will take longer to reach 0 again..
constexpr int32_t scalingfactor = 100; // to make sure we can do .1 decimal values with ints..
class AnalogInput : public Input
{
    using Input::Input;

private:
    // changed the imlementation to use a filtered value based on exponential filter

    int32_t average = 0;
    int32_t zeropostion=0;

public:
    void  init() override
    {
        average = 0;
        zeropostion = 0;
        for (int i = 0; i < nrzerosamples; i++)
        {
            delay(10);
            int32_t sample = (int32_t)analogRead(getPin()) * scalingfactor;
            zeropostion = (zeropostion * (CUT_OFF_frequency - 1) + sample) / CUT_OFF_frequency;
        }
        average = zeropostion;
        Serial.print("zeroposition : ");
        Serial.println(zeropostion);
    }
    inline uint16_t readInput(uint8_t pinnr)
    {

        // the input has a bias and is focusses around the 512 value (half of of the reference value)
        
        int32_t sample = (int32_t)analogRead(pinnr) * scalingfactor;

        uint8_t force = CUT_OFF_frequency;
        int32_t deviation = sample - zeropostion;
        if (deviation < 0)
        {
            sample = zeropostion - deviation;
        }

        if (sample < average)
        {
            force = decayfactor;
        }
        average = (average * (force - 1) + sample) / force;
        if (average < 0)
        {
            average = zeropostion;
        }
    //     Serial.print("pin: ");
    //     Serial.print(pinnr);
    // Serial.print(" sampl ");
    //     Serial.print(sample);
    //     Serial.print(" : ");

    //     Serial.print(zeropostion);
    //     Serial.print(" : ");
    //         Serial.print(" force :");
    //     Serial.print(force);
    //     Serial.print(" : ");
    //     Serial.print("avg: ");
    //     Serial.println(average);
        int32_t mv = (((average - zeropostion) * 5000) / scalingfactor) / 1023;
        
        if (mv < 0) mv = 0;
        if (mv > 5000) mv = 5000;
        
        return (uint16_t)mv;

    }
}; // AnalogInput