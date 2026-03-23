# RVBAlarm Watchdog & Input Freeze Analysis

## Problem Statement
The software periodically stops reacting to inputs and the watchdog timer does not trigger. Only a hard reset resolves the issue.

## Root Causes Identified

### 🔴 CRITICAL: millis() Rollover Bug in Input Polling

**Location:** [include/input.h](include/input.h#L33), [src/input.cpp](src/input.cpp#L36)

**Issue:**
- `oldmillis` is declared as `uint64_t` (64-bit) but `millis()` returns a 32-bit unsigned long on AVR platforms
- On Arduino Uno/Nano, `millis()` wraps around every ~49.7 days
- After rollover, the comparison `millis() >= (oldmillis + polltime)` at [src/input.cpp:33](src/input.cpp#L33) can remain **permanently false**
- This causes polling to stop indefinitely—inputs stop being read even though the main loop continues

**Impact:** Input events are completely missed; alarm cannot activate or deactivate

**Timeline:**
- Days 0-49: Works normally
- Day 49.71: millis() wraps from 4,294,967,295 to 0
- After wrap: `oldmillis` (large 32-bit copy) > `millis()` (reset to 0), condition fails for ~49.7 more days

---

### 🔴 CRITICAL: State Machine Timer Rollover

**Location:** [include/inputhandler.h](include/inputhandler.h#L46-L47), [src/inputhandler.cpp](src/inputhandler.cpp#L47,L71,L77,L85)

**Issue:**
- `blinkTime` and `flashtime` are `uint64_t`, compared against 32-bit `millis()`
- Same rollover vulnerability: after millis() wrap, timeout transitions stall
- State machine can become stuck in SETTING, ON, or RESETTING states indefinitely

**Impact:** If caught mid-alarm, system cannot recover to IDLE automatically; manual reset may be the only way out

---

### 🟡 SECONDARY: Watchdog Not Triggering Is Symptomatic, Not Causal

**Location:** [src/main.cpp:62](src/main.cpp#L62)

**Observation:**
- `wdt_reset()` is called every loop iteration at the end of `loop()`
- The main loop does NOT block or infinite-loop—it always completes and resets the watchdog
- Hence, the watchdog timer set to 250ms will **never expire** while the loop runs
- The system appears "frozen" (unresponsive) but is actually stuck in one state, still petting the watchdog

**Why this is problematic:** The watchdog cannot help if the freeze is a logic deadlock rather than a CPU hang

---

### 🟡 SECONDARY: Serial Logging Load

**Location:** [src/input.cpp](src/input.cpp#L44-50,L58-59,L66-69,L86-89), [src/inputhandler.cpp](src/inputhandler.cpp#L14-17,L146-171)

**Issue:**
- Frequent `Serial.print()` calls at 9600 baud (~1 ms per character)
- Bursts of logging can block loop timing and make state transitions unpredictable
- Heavy logging during polling can exceed the 5 ms poll interval

**Impact:** Degrades timing precision; can mask or exacerbate rollover problems

---

### 🟢 MINOR: Unnecessary pinMode Reconfiguration

**Location:** [include/digitalInput.h:13](include/digitalInput.h#L13)

**Issue:**
- `pinMode(pinnr, INPUT_PULLUP)` is called on **every single poll**
- Should be set once during initialization

**Impact:** Minimal performance cost, but indicates sloppy resource management

---

## Fix Strategy

1. **Change `uint64_t` to `uint32_t`** for all millis-based timers (eliminates rollover mismatch)
2. **Reduce unnecessary Serial output** (optional but improves timing reliability)
3. **Move pinMode to init phase** (cleanup)
4. **Consider safer timeout comparison using delta** (future improvement)

---

## Testing After Fix

- Run system continuously for 50+ days (simulate using `millis()` offset in simulator, or wait)
- Verify inputs and state transitions remain responsive
- Confirm watchdog behavior is meaningful (set reasonable timeout, verify it resets as expected)
