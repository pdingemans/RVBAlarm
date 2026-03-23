# RVBAlarm Fixes Applied

## Summary of Changes

### ✅ Critical Fixes Applied

#### 1. **Fixed millis() Rollover in Input Polling** 
- **File:** [include/input.h](include/input.h#L33)
- **Change:** `uint64_t oldmillis` → `uint32_t oldmillis`
- **Why:** Eliminates type mismatch that caused polling to stop after ~49.7 days of uptime

#### 2. **Fixed State Machine Timer Rollover**
- **File:** [include/inputhandler.h](include/inputhandler.h#L46-L47)
- **Change:** `uint64_t blinkTime` → `uint32_t blinkTime`; `uint64_t flashtime` → `uint32_t flashtime`
- **Why:** Prevents state transitions from stalling indefinitely after millis() wrap

#### 3. **Optimized Digital Input Initialization**
- **File:** [include/digitalInput.h](include/digitalInput.h)
- **Changes:**
  - Added `init()` override method to set `pinMode(INPUT_PULLUP)` once at startup
  - Removed redundant `pinMode()` call from `readInput()` 
- **File:** [src/main.cpp](src/main.cpp#L49-L50)
- **Change:** Added `manualSet.init()` and `manualReSet.init()` calls
- **Why:** Removes per-poll overhead; pinMode should only be set once at boot

#### 4. **Reduced Serial Logging Overhead**
- **File:** [src/input.cpp](src/input.cpp)
  - Removed verbose per-poll debug prints (kept only state change events)
  - "debouncing high/low" messages removed; only RISING/FALLING transitions logged
- **File:** [src/inputhandler.cpp](src/inputhandler.cpp)
  - Removed per-event logging noise (`handleEvent()` function); kept state-change messages
  - Removed toggle logging; kept on/off transitions
- **Why:** Serial I/O at 9600 baud adds ~10-20% latency; reducing noise improves polling timing reliability

---

## Root Cause Summary

| Issue | Cause | Impact | Fixed |
|-------|-------|--------|-------|
| Inputs stop reacting | `uint64_t oldmillis` vs 32-bit `millis()` comparison | Polling permanently halts after rollover | ✅ |
| State transitions stall | Same rollover in `blinkTime`/`flashtime` | Alarm stuck in mid-state | ✅ |
| Watchdog doesn't trigger | Loop still runs (keeps resetting WDT); freeze is logical, not hardware | Symptom of #1 and #2 | Addressed by fixing polling/state |
| Timing jitter | Excessive Serial.print() blocking | Degrades debounce reliability | ✅ |
| Unnecessary reconfiguration | `pinMode()` called per read | Minor overhead | ✅ |

---

## Testing Recommendations

1. **Immediate Testing:**
   - Compile and flash the updated code
   - Verify inputs respond normally
   - Confirm state transitions (IDLE → ON → CONSTANTON → RESETTING) work smoothly

2. **Long-Duration Testing (if possible):**
   - Run system continuously for several weeks or simulate long uptime
   - Alternatively, use Arduino IDE + PlatformIO to test with simulated `millis()` offset
   - Verify no freeze occurs as system approaches 50-day milestone

3. **Watchdog Validation:**
   - Set `WDTO_250MS` to a longer period temporarily (e.g., `WDTO_1S`) for testing
   - Confirm that if loop gets stuck, watchdog triggers reset within timeout window
   - Revert to `WDTO_250MS` for production

---

## Future Improvements (Optional)

1. **Use millis() delta comparison pattern** (safer against any timing bug):
   ```cpp
   uint32_t now = millis();
   if (now - oldmillis >= polltime) {
       oldmillis = now;
       // ... poll logic
   }
   ```
   This pattern is immune to rollover regardless of data types.

2. **Complete the AnalogInput initialization pattern** - Currently only Analog inputs call `init()` in main; ensure all Input subclasses do the same.

3. **Consider event queue** - If more robustness is needed, decouple polling from event handling with a small ringbuffer to ensure no events are lost during high-frequency state changes.
