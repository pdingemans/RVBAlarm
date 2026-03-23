#include "..\include\inputhandler.h"
#include "..\include\actuator.h"

// Alert/Notice timing constants
constexpr uint16_t beeponofftime = 500;           // 500ms on/off for alert beep
constexpr uint16_t beeptime = 2000;               // 2000ms total beep duration

// CONSTANTON state timing constants
constexpr uint16_t lampConstantonOnTime = 1000;   // Lamp ON duration in CONSTANTON (0.5 Hz)
constexpr uint16_t lampConstantonOffTime = 1000;  // Lamp OFF duration in CONSTANTON (0.5 Hz)
constexpr uint16_t sireneConstantonOnTime = 1000; // Sirene ON duration in CONSTANTON (1 second)
constexpr uint32_t sireneConstantonOffTime = 60000; // Sirene OFF duration in CONSTANTON (60 seconds)

void Inputhandler::handleEvent(Event evt)
{
  switch (state)
  {
  case IDLE:
    if (evt == MANUALSET)
    {
      lampActuator.setTiming(beeponofftime, beeponofftime);
      lampActuator.setBlinking();
      sireneActuator.setTiming(beeponofftime, beeponofftime);
      sireneActuator.setBlinking();
      flashtime = millis() + beeptime;
      state = SETTING;
      Serial.println("manual set started");
    }
    else if (evt == SETSTARTED)
    {
      lampActuator.setOn();
      sireneActuator.setTiming(beeponofftime, beeponofftime);
      sireneActuator.setBlinking();
      state = ON;
    }

    break;

  case ON:
    if (evt == SETSTOPPED)
    {
      // Enter CONSTANTON: lamp blinks at 0.5 Hz, sirene blinks 1s on / 60s off
      lampActuator.setTiming(lampConstantonOnTime, lampConstantonOffTime);
      lampActuator.setBlinking();
      sireneActuator.setTiming(sireneConstantonOnTime, sireneConstantonOffTime);
      sireneActuator.setBlinking();
      state = CONSTANTON;
    }

    break;
  

  case CONSTANTON:
    if (evt == MANUALRESET || evt == RESET)
    {
      lampActuator.setTiming(beeponofftime, beeponofftime);
      lampActuator.setBlinking();
      sireneActuator.setTiming(beeponofftime, beeponofftime);
      sireneActuator.setBlinking();
      flashtime = millis() + beeptime;
      Serial.println(" reset started");
      state = RESETTING;
    }
    else if (evt == SETSTARTED)
    {
      lampActuator.setOn();
      sireneActuator.setTiming(beeponofftime, beeponofftime);
      sireneActuator.setBlinking();
      state = ON;
    }
    
    break;

  case SETTING:
    if (millis() >= flashtime)
    {
      // Enter CONSTANTON: lamp blinks at 0.5 Hz, sirene blinks 1s on / 60s off
      lampActuator.setTiming(lampConstantonOnTime, lampConstantonOffTime);
      lampActuator.setBlinking();
      sireneActuator.setTiming(sireneConstantonOnTime, sireneConstantonOffTime);
      sireneActuator.setBlinking();
      state = CONSTANTON;
    }
    break;

  case RESETTING:
    if (millis() >= flashtime)
    {
      sireneActuator.setOff();
      lampActuator.setOff();
      state = IDLE;
    }
    break;

  default:

    state = IDLE;
    lampActuator.setOff();
    sireneActuator.setOff();
  }
}
Inputhandler::Inputhandler(Input &setInput, Input &resetInput, Input &manualSet, Input &manualReset)
    : set(setInput),
      reset(resetInput),
      manualSet(manualSet),
      manualReset(manualReset)
{
  state = IDLE;
  lampActuator.setOff();
  sireneActuator.setOff();
}
void Inputhandler::handle()
{
  uint8_t airportCalling = set.getStatus();
  
  if (airportCalling == RISING)
  {
    handleEvent(SETSTARTED);
  }
  else if (airportCalling == FALLING)
  {
    handleEvent(SETSTOPPED);
  }
  else if (reset.getStatus() == RISING)
  {
    handleEvent(RESET);
  }
  else if (manualSet.getStatus() == RISING)
  {
    handleEvent(MANUALSET);
  }
  else if (manualReset.getStatus() == RISING)
  {
    handleEvent(MANUALRESET);
  }
  else
  {
    handleEvent(NONE); // check the timer...
  }
}
