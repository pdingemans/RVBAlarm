/*
RVB alarm

*/
#include "..\include\inputhandler.h"
#include "..\include\analogInput.h"
#include "..\include\digitalInput.h"
// definitions for analoginputs
constexpr uint8_t sensorPinAR = A0; // select the input pin for Audio Right
constexpr uint8_t sensorPinAL = A1; // select the input pin for Audio Left
constexpr uint16_t threshold = 15;  // an arbitrary threshold level that's in the range of the analog input
constexpr uint8_t timeBetweenPolls = 0;
constexpr uint8_t nrHighPolls = 5;
constexpr uint16_t inactiveTime = 1000; // 2poll time *500 msec before input goed inactive

// definitions for digital inputs
constexpr uint8_t pinSet = 3;
constexpr uint8_t pinReset = 4;
constexpr uint16_t thresholddigital = 1;       // must be 1 as reading a high will return 1
constexpr uint8_t timeBetweenPollsdigital = 5; // 5 msec beteen polls
constexpr uint8_t nrHighPollsdigital = 10;     // at least 50 msec high to fire the input once high
constexpr uint16_t inactiveTimedigital = 10;   // at least 50 msec low to fire input once low

// create the objects we're going to use
AnalogInput setInput(sensorPinAR, threshold, timeBetweenPolls, nrHighPolls, inactiveTime);
AnalogInput resetInput(sensorPinAL, threshold, timeBetweenPolls, nrHighPolls, inactiveTime);
DigitalInput manualSet(pinSet, thresholddigital, timeBetweenPollsdigital, nrHighPollsdigital, inactiveTimedigital);
DigitalInput manualReSet(pinReset, thresholddigital, timeBetweenPollsdigital, nrHighPollsdigital, inactiveTimedigital);
Inputhandler myHandler(setInput, resetInput, manualSet, manualReSet);

void setup()
{

  // the setup routine runs once when you press reset:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop()
{
  // read the value from the sensor:

  setInput.poll();
  resetInput.poll();
  manualReSet.poll();
  manualSet.poll();
  myHandler.handle();

  // Serial.print(sensorValueAL);
  // Serial.print("     ");
  // Serial.println(sensorValueAR);
  // if the analog value is high enough, turn on the LED:
  /* if (sensorValueAR > threshold) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(sirene, HIGH);
    Serial.println("Vliegverbod Actief");
    Serial.println("Sirene aan");
    delay(1000);
  } else {
      if (sirenestatus=1) {
      digitalWrite(sirene, LOW);
    Serial.println("Sirene uit");
      }
      else { }
    }
  if (sensorValueAL > threshold) {
    digitalWrite(ledPin, LOW);
    Serial.println("Vliegverbod Opgeheven");
    delay(1000);
  } else { }
  // stop the program for for 100 milliseconds:
  delay(100);
  */
}
