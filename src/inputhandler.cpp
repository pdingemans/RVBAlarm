#include "inputhandler.h"

Inputhandler::Inputhandler(Input &setInput, Input &resetInput, Input &manualSet, Input &manualReset)
    : set(setInput),
      reset(resetInput),
      manualSet(manualSet),
      manualReset(manualReset)
{
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  pinMode(sirenePin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(sirenePin, LOW);
}
void Inputhandler::handle()
{
 
  if(set.getStatus()==RISING)
  {
    handleEvent(SETSTARTED);
  }
  if (set.getStatus()==FALLING)
  {
    handleEvent(SETSTOPPED)
  }
  if(reset.getStatus()==RISING)
  {
    handleEvent(RESET);
  }
  if (manualSet)

}
void Inputhandler::handleEvent(Event evt)
{
}
void Inputhandler::lampOn()
{
}
void Inputhandler::lampOff()
{
}
void Inputhandler::sireneOn()
{
}
void Inputhandler::sireneOff()
{
}
void Inputhandler::setTimer()
{
}
void Inputhandler::setFlashtimer()
{
}
bool Inputhandler::timerElapsed()
{
  return false;
}
bool Inputhandler::flashtimerElapsed()
{
  return false;
}
