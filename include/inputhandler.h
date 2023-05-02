#pragma once
#include <Arduino.h>
#include <stdint.h>
#include "input.h"

constexpr uint8_t ledPin = 13;   // select the pin for the LED
constexpr uint8_t sirenePin = 6; // select the pin for the Sirene

enum class InputStatus
{
    IDLE,
    ACTIVE
};
class Inputhandler
{
public:
    Inputhandler(Input &setInput, Input &resetInput, Input &manualSet, Input &manualReset);
    Inputhandler(const Input &&, const Input &&, const Input &&, const Input &&) = delete; // prevents rvalue binding
    InputStatus getStatus();
    void handle();

private:
    enum State
    {
        IDLE,
        ACTIVE,
        SWITCHINGOFF,
    };
    State state = State::IDLE;
    InputStatus status = InputStatus::IDLE;
    const Input &set;
    const Input &reset;
    const Input &manualSet;
    const Input &manualReset;
};