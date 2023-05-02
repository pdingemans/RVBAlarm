#include "inputhandler.h"

Inputhandler::Inputhandler(Input &setInput, Input &resetInput, Input &manualSet, Input &manualReset)
:set(setInput),reset(resetInput),manualSet(manualSet),manualReset(manualReset)
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