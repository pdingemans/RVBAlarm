#pragma once
#include <Arduino.h>



class Input
{
public:
    Input(uint8_t pinnr, uint16_t threshold,uint16_t thresholdLow, uint16_t poltime,uint8_t counts,uint16_t inactiveTime);
    uint8_t getStatus() const;
    void poll();
    inline uint16_t getThreshold() {return threshold;};
    inline uint8_t getPin() const { return pin; };

    virtual uint16_t readInput(uint8_t pinnr) = 0;
    virtual void init(){};
 private:
    enum State
    {
        INACTIVE,
        DEBOUNCING,
        ACTIVE,
        DEBOUNCINGLOW
    } state = INACTIVE;
    mutable uint8_t  event=0;
 
    uint8_t pin;
    uint16_t threshold;
    uint16_t thresholdLow;
    uint16_t polltime;
    uint8_t minimalActiveCounts;
    uint16_t inactiveTime=0;
    uint64_t oldmillis=0;
    uint16_t sensorValue=0;
    uint8_t activeCount = 0;
};