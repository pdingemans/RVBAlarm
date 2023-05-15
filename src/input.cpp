#include "..\include\input.h"

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
    event = 0;
    curwaittime = polltime;
}

uint8_t Input::getStatus() const
{
    uint8_t retval = (uint8_t) event;
    event = 0; // we only return true once per active cycle
       return (retval);
}

void Input::poll()
{

    // the pin needs to be at least a couple of time above the threshold to prevent
    // spurious triggers
    if (millis() >= (oldmillis + curwaittime))
    {
        uint8_t val = readInput(pin);
        oldmillis = millis();
        switch (state)
        {
        case INACTIVE:
            curwaittime = polltime;

            activeCount = 0;
            if (val == HIGH)
            {
                state = DEBOUNCING;
                // Serial.print("debouncing ");
                // Serial.println(pin);
            }
            break;
        case DEBOUNCING:
            if (val == HIGH)
            {
                activeCount++;
                if (activeCount >= minimalActiveCounts)
                {
                    activeCount = 0;
                    state = ACTIVE;
                    event = RISING;
                    curwaittime = inactiveTime;
                    // Serial.print("pin high :  ");
                    // Serial.println(pin);
               }
            }
            else
            {
                state = INACTIVE;
            }
            break;
        case ACTIVE:
            if (val == LOW)
            {
                activeCount++;
                if (activeCount >= minimalActiveCounts)
                {
                    activeCount = 0;
                    event = FALLING;
                    state = INACTIVE;
                    // Serial.print("debouncing low : ");
                    // Serial.println(event);
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
