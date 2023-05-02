The state diagram below depict the behavior of the alarm.

```mermaid
stateDiagram-v2
    [*] --> Idle : /Initialize
    Idle --> Alarm : SetAlarm / SetFlashTimer
    state Alarm {
      [*]-->Flashing
      state Flashing {
        [*]-->ON : /Lampon, SireneOn, setTimer
        ON-->OFF : TimerElapsed/Lampoff, SireneOff, setTimer
        OFF-->ON : TimerElasped/Lampon, SireneOn, setTimer
      }
      Flashing --> On : FlashTimerElapsed/Lampon,SireneOff
    }
    Alarm -->Alarm : SetAlarm/SetFlashTimer
    Alarm --> Idle : ResetAlarm/Lampoff,SireneOff
```

and here the behaviour of the input debouncer

```mermaid
stateDiagram-v2

  
    [*] --> Inactive : /Initialize
    Inactive-->Debouncing : inputactive
    Debouncing -->Inactive : inputNOK
    Debouncing --> Active: inputOK/startTimer,evt(InputActive)
    Active --> Inactive : timerElapsed
 
```