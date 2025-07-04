#include "..\include\input.h"
#include <Arduino.h>
#include <stdint.h>

Input::Input(uint8_t pinnr, uint16_t threshold, uint16_t thresholdLow, uint16_t polltime, uint8_t counts, uint16_t inactiveTime) : pin(pinnr),

                                                                                                                                   threshold(threshold),
                                                                                                                                   thresholdLow(thresholdLow),
                                                                                                                                   polltime(polltime),
                                                                                                                                   minimalActiveCounts(counts),
                                                                                                                                   inactiveTime(inactiveTime)
                                                                                                                                
{
 

    pinMode(pin, INPUT);
    oldmillis = millis();

}

uint8_t Input::getStatus() const
{
    const auto retval = (uint8_t)event;
    event = 0; // we only return true once per active cycle
    return (retval);
}

void Input::poll()
{

    // the pin needs to be at least a couple of time above the threshold to prevent
    // spurious triggers
    if (millis() >= (oldmillis + polltime))
    {
        const uint16_t val = readInput(pin);
        oldmillis = millis();

        switch (state)
        {
        case INACTIVE:
            activeCount = 0;
            if (val >= threshold)
            {
                Serial.print(val);
                Serial.print(" : ");
                state = DEBOUNCING;
                Serial.print("debouncing high");
                Serial.print("activecount: ");
                Serial.print(activeCount);
                Serial.println(pin);

            }
            break;
        case DEBOUNCING:
            if (val >= threshold)
            {
                activeCount++;
                Serial.print("activecount: ");
                Serial.println(activeCount);
                if (activeCount >= minimalActiveCounts)
                {
                    activeCount = 0;
                    state = ACTIVE;
                    event = RISING;
 
                    Serial.print(val);
                    Serial.print(" : ");
                    Serial.print("pin high :  ");
                    Serial.println(pin);
                }
            }
            else if (val <= thresholdLow) // hysterese if needed....
            {
                state = INACTIVE;
            }
            break;
        case ACTIVE:
            if (val <= thresholdLow)
            {
                activeCount++;
                if (activeCount >= inactiveTime)
                {
                    activeCount = 0;
                    event = FALLING;
                    state = INACTIVE;
                    Serial.print(val);
                    Serial.print(" : ");
                    Serial.print("debouncing low : ");
                    Serial.println(pin);
                }
            }
            else
            {
                activeCount = 0; // start over again if we have a spurious high
            }

            break;
        default:
            state = INACTIVE;
            break;
        }
    }
}
