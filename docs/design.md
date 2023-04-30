```mermaid
stateDiagram-v2
    [*] --> Idle : /Initialize
    Idle --> Alarm : SetAlarm / SetFlashTimer
    state Alarm {
      [*]-->flashing
      state Flashing
      {
        [*]-->ON : /Lampon, setTimer
        ON-->OFF : TimerElapsed/Lampoff, setTimer
        OFF-->ON : TimerElasped/Lampon,setTimer
      }
      flashing --> On : FlashTimerElapsed/Lampon
    }
    Alarm --> Idle : ResetAlarm/Lampon
```
