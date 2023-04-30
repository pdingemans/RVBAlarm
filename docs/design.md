```mermaid
stateDiagram-v2
    [*] --> Idle : /Initialize
    Idle --> Alarm : SetAlarm
    state Alarm {
      [*]flashing
      flashing --> On
    }
    Alarm --> Idle : ResetAlarm
```
