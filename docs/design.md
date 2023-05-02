The state diagram below depict the behavior of the alarm.

```mermaid
stateDiagram-v2

    [*] --> IDLE : /Initialize

    IDLE-->ALARM : SetAlarm 
    IDLE-->ALARM : ManualSet
    state ALARM {
    [*]-->ON :  / SetFlashTimer, Lampon, SireneOn, setTimer
    ON-->OFF : TimerElapsed/Lampoff, SireneOff, setTimer
    OFF-->ON : TimerElasped/Lampon, SireneOn, setTimer
    OFF-->CONSTANTON : FlashTimerElapsed/Lampon,SireneOff
       }
    ALARM-->SWITCHINGOFF : ResetAlarm
    SWITCHINGOFF-->IDLE : FlashTimerElapsed/lampoff,SireneOff
    state SWITCHINGOFF {
    [*]-->SWON : /SetFlashTimer,lampoff,SireneOn
    SWON-->SWOFF : TimerElapsed/SetFlashTimer,SireneOff
    SWOFF-->SWON: TimerElapsed/SetFlashTimer,SireneOn
    }
    ALARM-->ALARM : SetAlarm 
    ALARM-->IDLE : ManualReset/lampoff,SireneOff
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