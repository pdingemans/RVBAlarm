#include "inputhandler.h"

constexpr uint8_t ledPin = 13;   // select the pin for the LED
constexpr uint8_t sirenePin = 6; // select the pin for the Sirene

constexpr uint16_t beeponofftime = 500;

constexpr uint16_t beeptime = 2000;

void Inputhandler::handleEvent(Event evt)
{
  if (evt != NONE)
  {
    Serial.print("handling event:");
    Serial.print(evt);
    Serial.print("in state: ");
    Serial.println(state);
  }
  switch (state)
  {
  case IDLE:
    if (evt == MANUALSET)
    {
      // lets blink a while with 50% dutycycle
      blinkTime = millis() + beeponofftime;
      flashtime = millis() + beeptime;
      state = SETTING;
      Serial.println("manual set started");
    }
    else if (evt == SETSTARTED)
    {
      sireneOn();
      lampOn();
      blinkTime = millis() + beeponofftime;
      state = ON;
    }

    break;

  case ON:
    // ringing has ended
    if (evt == SETSTOPPED)
    {
      state = CONSTANTON;
      sireneOff();
    }
    else if (millis() >= blinkTime)
    {
      // we are still rining, so check if we need to toggle the sirene
      blinkTime = millis() + beeponofftime;
      sireneToggle();
    }

    break;
  

  case CONSTANTON:
    if (evt == MANUALRESET || evt == RESET)
    {
      blinkTime = millis() + beeponofftime;
      flashtime = millis() + beeptime;

      Serial.println(" reset started");
      sireneOn();
      state = RESETTING;
    }
    break;

  case SETTING:

    if (millis() >= flashtime)
    {
      lampOn();
      sireneOff();
      state = CONSTANTON;
    }
    else if (millis() >= blinkTime)
    {
      blinkTime = millis() + beeponofftime;
      sireneToggle();
    }
    break;

  case RESETTING:
    if (millis() >= blinkTime)
    {
      sireneOff();
      lampOff();
      state = IDLE;
    }
    break;

  default:

    state = IDLE;
    lampOff();
    sireneOff();
  }
}
Inputhandler::Inputhandler(Input &setInput, Input &resetInput, Input &manualSet, Input &manualReset)
    : set(setInput),
      reset(resetInput),
      manualSet(manualSet),
      manualReset(manualReset)
{
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  pinMode(sirenePin, OUTPUT);
  state = IDLE;
  lampOff();
  sireneOff();
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

void Inputhandler::lampOn()
{
  digitalWrite(ledPin, HIGH);
  Serial.println("lamp on");
}
void Inputhandler::lampOff()
{
  digitalWrite(ledPin, LOW);
  Serial.println("lamp off");
}
void Inputhandler::lampToggle()
{
  digitalWrite(ledPin, !digitalRead(ledPin));
  Serial.println("lamp toggle");
}
void Inputhandler::sireneOn()
{
  digitalWrite(sirenePin, HIGH);
  Serial.println("Sirene on");
}
void Inputhandler::sireneOff()
{
  digitalWrite(sirenePin, LOW);
  Serial.println("Sirene off");
}
void Inputhandler::sireneToggle()
{
  digitalWrite(sirenePin, !digitalRead(sirenePin));
  Serial.println("Sirene toggle");
}
