#pragma once
#include <Arduino.h>
#include <stdint.h>


class Input
{
public:
    Input(uint8_t pinnr, uint16_t threshold,uint16_t poltime,uint8_t counts,uint16_t inactiveTime);
    uint8_t getStatus() const;
    void poll();
    inline uint16_t getThreshold() {return threshold;};
private:
    virtual uint8_t readInput(uint8_t pinnr) = 0;
   
 
    enum State
    {
        INACTIVE,
        DEBOUNCING,
        ACTIVE
    } state;
    mutable bool  event;
 
    uint8_t pin;
    uint16_t threshold;
    uint16_t sensorValue;
    uint8_t activeCount;
    uint16_t curwaittime;
    
    uint64_t oldmillis;
    uint16_t polltime;
    uint8_t minimalActiveCounts;
    uint16_t inactiveTime;
};