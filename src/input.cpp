#include "input.h"

Input::Input(uint8_t pinnr, uint16_t threshold, uint16_t polltime, uint8_t counts, uint16_t inactiveTime) : pin(pinnr),
                                                                                                            threshold(threshold),
                                                                                                            polltime(polltime),
                                                                                                            minimalActiveCounts(counts),
                                                                                                            inactiveTime(inactiveTime)
{
    oldmillis = millis();
    activeCount = 0;
    pinMode(pin, INPUT);
    state = State::INACTIVE;
    event = false;
    curwaittime = polltime;
}

uint8_t Input::getStatus()
{
    bool retval = event;
    event = false; // we only return true once per active cycle
    return (retval);
}

void Input::poll()
{

    // the pin needs to be at least a couple of time above the threshold to prevent
    // spurious triggers
    if (millis() >= (oldmillis + curwaittime))
    {
        uint16_t val = readInput(pin);
        oldmillis = millis();
        switch (state)
        {
        case INACTIVE:
            curwaittime = polltime;
            event = false;
            activeCount = 0;
            if (val >= threshold)
            {
                state = DEBOUNCING;
            }
            break;
        case DEBOUNCING:
            if (val >= threshold)
            {
                activeCount++;
                if (activeCount >= minimalActiveCounts)
                {
                    state = ACTIVE;
                    event = true;
                    curwaittime = inactiveTime;
                }
            }
            else
            {
                state = INACTIVE;
            }
            break;
        case ACTIVE:
            state = INACTIVE;
            break;
        default:
            state = INACTIVE;
            break;
        }
    }
}
