#pragma once
#include <Arduino.h>
#include <stdint.h>


class Input
{
public:
    Input(uint8_t pinnr, uint16_t threshold,uint8_t poltime,uint8_t counts);
    uint8_t getStatus();
    void poll();

private:

    uint8_t pin;
    uint16_t threshold;
    uint16_t sensorValue;
    uint8_t activeCount;

    uint64_t oldmillis;
    uint8_t polltime;
    uint8_t minimalActiveCounts;
};