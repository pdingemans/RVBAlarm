#pragma once
#include "input.h"

#include <Arduino.h>



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
        ON,
        OFF,
        CONSTANTON,
        SETTING,
        RESETTING

    };
    enum Event
    {
        NONE,
        MANUALRESET,
        MANUALSET,
        SETSTARTED,
        SETSTOPPED,
        RESET,

    };
    State state = State::IDLE;
    void handleEvent(Event evt);
    void lampOn();
    void lampOff();
    void lampToggle();
    void sireneOn();
    void sireneOff();
    void sireneToggle();

    uint64_t blinkTime;
    uint64_t flashtime;

    const Input &set;
    const Input &reset;
    const Input &manualSet;
    const Input &manualReset;
};