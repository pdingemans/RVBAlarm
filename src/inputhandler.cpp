#include "inputhandler.h"

Inputhandler::Inputhandler(Input &setInput, Input &resetInput)
:set(setInput),reset(resetInput)
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