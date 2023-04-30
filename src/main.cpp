/*
RVB alarm

*/
#include "inputhandler.h"
 
Input inputA(sensorPinAR,threshold,timeBetweenPolls,nrHighPolls);
Input inputB(sensorPinAL,threshold,timeBetweenPolls,nrHighPolls);
Inputhandler myHandler;


void setup() {

  // the setup routine runs once when you press reset:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  inputA.poll();
  inputB.poll();
  
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
