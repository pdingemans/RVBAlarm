#pragma once
#include <Arduino.h>
#include <stdint.h>
constexpr uint8_t sensorPinAR = A0; // select the input pin for Audio Right
constexpr uint8_t sensorPinAL = A1; // select the input pin for Audio Left
constexpr uint8_t ledPin = 13;      // select the pin for the LED
constexpr uint8_t sirenePin = 6;    // select the pin for the Sirene

constexpr uint8_t threshold = 100; // an arbitrary threshold level that's in the range of the analog input
enum class InputStatus
{
    IDLE,
    ACTIVE
};
class Inputhandler
{
public:
    Inputhandler();
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
    uint16_t sensorValueAR = 0; // variable to store the value coming from the sensor
    uint16_t sensorValueAL = 0; // variable to store the value coming from the sensor
};