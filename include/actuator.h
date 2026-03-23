#pragma once
#include <Arduino.h>

class Actuator
{
private:
    enum Mode { OFF, ON, BLINKING };

public:
    Actuator(unsigned char pin, uint16_t offTime, uint16_t onTime);
    
    void setOn();
    void setOff();
    void setBlinking();
    void setTiming(uint16_t offTime, uint16_t onTime);
    void update();
    
    bool isOn() const;
    bool isBlinking() const;

private:
    unsigned char pin;
    uint16_t offTime;
    uint16_t onTime;
    Mode mode;
    bool pinState;
    uint32_t lastTransition;
    
    void setPinState(bool state);
};

extern Actuator sireneActuator;
extern Actuator lampActuator;
