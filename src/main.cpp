/*
RVB alarm

*/

#include <avr/wdt.h>
#include "..\include\inputhandler.h"
#include "..\include\analogInput.h"
#include "..\include\digitalInput.h"
// definitions for analoginputs
constexpr uint8_t sensorPinAR = A1;  // select the input pin for Audio Right
constexpr uint8_t sensorPinAL = A0;  // select the input pin for Audio Left
constexpr uint16_t threshold = 2;    // 25 seems to work well with the 25 samples and wavs
constexpr uint16_t thresholdLow = 0; // hysterese...

// increase high edge detection to 250 msec. 5 msec was too short..
constexpr uint8_t timeBetweenPolls = 5; // was 1
constexpr uint8_t nrHighPolls = 50;     // amount of polls the signal needs to be active before going high  , was 5
constexpr uint16_t inactiveTime = 10;   // amount of polls the signal needs to be inactive before going low

// definitions for digital inputs
constexpr uint8_t pinSet = 3;
constexpr uint8_t pinReset = 4;
constexpr uint16_t thresholddigital = 1;     // must be 1 as reading a high will return 1
constexpr uint16_t thresholddigitalLow = 01; // must be 1 as reading a high will return 1

constexpr uint8_t timeBetweenPollsdigital = 5; // 5 msec beteen polls
constexpr uint8_t nrHighPollsdigital = 10;     // at least 50 msec high to fire the input once high
constexpr uint16_t inactiveTimedigital = 10;   // at least 50 msec low to fire input once low

// create the objects we're going to use
AnalogInput setInput(sensorPinAR, threshold, thresholdLow, timeBetweenPolls, nrHighPolls, inactiveTime);
AnalogInput resetInput(sensorPinAL, threshold, thresholdLow, timeBetweenPolls, nrHighPolls, inactiveTime);
DigitalInput manualSet(pinSet, thresholddigital, thresholddigitalLow, timeBetweenPollsdigital, nrHighPollsdigital, inactiveTimedigital);
DigitalInput manualReSet(pinReset, thresholddigital, thresholddigitalLow, timeBetweenPollsdigital, nrHighPollsdigital, inactiveTimedigital);
Inputhandler myHandler(setInput, resetInput, manualSet, manualReSet);

void setup()
{

  // the setup routine runs once when you press reset:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("-----  resetting with watchdog on------");

  delay(500);
  setInput.init();
  resetInput.init();  
  wdt_enable(WDTO_250MS);

}
void loop()
{

  //setInput.poll();
  resetInput.poll();
  //manualReSet.poll();
  //manualSet.poll();
  //myHandler.handle();

  // Reset the watchdog timer
  wdt_reset();

}
