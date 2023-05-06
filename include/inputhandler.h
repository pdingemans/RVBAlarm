#pragma once
#include <Arduino.h>
#include <stdint.h>
#include "input.h"

constexpr uint8_t ledPin = 13;   // select the pin for the LED
constexpr uint8_t sirenePin = 6; // select the pin for the Sirene


class Inputhandler
{
public:
    Inputhandler(Input &setInput, Input &resetInput, Input &manualSet, Input &manualReset);
   // Inputhandler(const Input &&, const Input &&, const Input &&, const Input &&) = delete; // prevents rvalue binding
   
    void handle();

private:
    enum State
    {
        IDLE,
        ALARM,
        ON,
        OFF,
        CONSTANTON,
        SWITCHINGOFF,
        SWON,
        SWOFF
    };
    enum Event
    {
        MANUALRESET,
        MANUALSET,
        SET,
        RESET,
        TIMERELAPSED,
        FLASHTIMERELAPSED
    };
    State state = State::IDLE;
    void handleEvent(Event evt);
    void lampOn();
    void lampOff();
    void sireneOn();
    void sireneOff();
    void setTimer();
    void setFlashtimer();
    bool timerElapsed();
    bool flashtimerElapsed();
   
    uint64_t oldTime;
    uint16_t blinkTime;
    uint16_t flashtime;
    uint64_t oldFlashTime;
    const Input &set;
    const Input &reset;
    const Input &manualSet;
    const Input &manualReset;
};