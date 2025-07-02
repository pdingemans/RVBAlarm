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
        //lets assume we have no inout signal when the system starts
        //we can find the zero position by averaging the first 100 samples
        //and use that to determine negative signals, which we then make positive
        // this way we make a software bridge rectifier
        for (int i = 0; i < nrzerosamples; i++) // magic nuymber for now.. this is really bad....
        {
            delay(10); // wait a bit to make sure we have a stable value
            zeropostion = zeropostion + ((analogRead(getPin()) * scalingfactor - zeropostion) / CUT_OFF_frequency);
        }
        average = zeropostion; // we assume the average is the zeroposition
    }
    inline uint16_t readInput(uint8_t pinnr)
    {

        // the input has a bias and is focusses around the 512 value (half of of the reference value)
        
        int32_t sample = (int32_t)analogRead(pinnr) * scalingfactor;

        uint8_t force = CUT_OFF_frequency;
        if (sample < zeropostion)
        {
            sample = zeropostion + (zeropostion - sample); // we flip "negative" values around the zero position, (our sw bridge rectifier)
        }
        // Serial.print("pin");
        // Serial.print(pinnr);
        // Serial.print(":");
        // Serial.print("idx");
        // Serial.print(idx);
        // Serial.print(":");
        // Serial.print(samples[idx]);
        // Serial.print(":");
        if (sample < average)
        {
            force = decayfactor;
        }
        average = average + ((sample - average) / force);
        // this should not happen, so to be sure set it to 0.....
        if (average < 0)
        {
            average = zeropostion; // kake sure we do not get negative values
        }
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
    //     ;
        return ((average - zeropostion)/scalingfactor); //>= getThreshold() ? HIGH : LOW;

    }
}; // AnalogInput