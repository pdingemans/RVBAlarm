#include "input.h"

Input::Input(uint8_t pinnr, uint16_t threshold, uint8_t polltime,uint8_t counts)
    : pin(pinnr), threshold(threshold), polltime(polltime),minimalActiveCounts(counts)
{
    oldmillis = millis();
    activeCount = 0;
    pinMode(pin, INPUT);
}

uint8_t Input::getStatus()
{
    return (activeCount >= minimalActiveCounts);
}
void Input::poll()
{
    uint16_t val = analogRead(pin);
    // the pin needs to be at least a couple of time above the threshold to prevent
    // spurious triggers
    if (millis() >= (oldmillis + polltime))
    {
        oldmillis = millis();
        if (val > threshold)
        {
            // lets simple debounce a bit
            // count up, but only if we didnt reach te max
            
            if(activeCount < minimalActiveCounts )  
            {
                activeCount++;
            }
        }
    }
}
