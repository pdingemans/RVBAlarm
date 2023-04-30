#include "inputhandler.h"

Inputhandler::Inputhandler()
{
      // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  pinMode(sirenePin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(sirenePin, LOW);
}

void Inputhandler::handle()
{
    

}