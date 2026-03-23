# RVBAlarm Design Patterns Analysis

## Executive Summary

The RVBAlarm software demonstrates a **mixed architectural approach** with some excellent design patterns alongside problematic anti-patterns. Overall: **65% Good, 35% Problematic**.

---

## ✅ GOOD DESIGN PATTERNS

### 1. **Template Method Pattern** (EXCELLENT)
**Location:** `include/input.h`, `src/input.cpp`

```cpp
class Input {
    virtual uint16_t readInput(uint8_t pinnr) = 0;  // Concrete subclasses implement
    virtual void init() { };                        // Optional subclass override
    void poll() { /* Common debouncing SM */ }      // Template method
};

class AnalogInput : public Input { /* Override readInput */ };
class DigitalInput : public Input { /* Override readInput */ };
```

**Why Good:**
- ✅ Encapsulates reusable debouncing algorithm in base class
- ✅ Subclasses only implement sensor-specific `readInput()`
- ✅ Single Responsibility Principle perfectly applied
- ✅ Easy to add new sensor types (I2C, WiFi, etc.) without modifying debouncing logic

**Score:** 10/10 - Textbook example of Template Method

---

### 2. **State Machine Pattern** (EXCELLENT)
**Location:** Multiple files

**Input Debouncing State Machine:**
```
INACTIVE → DEBOUNCING → ACTIVE → DEBOUNCINGLOW → INACTIVE
```
- Hysteretic threshold prevents bouncing
- Clear transitions with named states
- Debouncing parameters (counts, thresholds) configurable

**Control State Machine (Inputhandler):**
```
IDLE ↔ SETTING → ON → CONSTANTON → RESETTING → IDLE
```
- 6 states with explicit events
- Timer-based transitions
- Clear entry/exit actions

**Why Good:**
- ✅ Easy to understand and visualize (see `docs/state_machine.puml`)
- ✅ Exhaustive case coverage prevents undefined behavior
- ✅ State-specific behavior isolated in switch cases
- ✅ Transitions are explicit and trackable

**Score:** 9/10 - Well-structured, minor: could document state invariants

---

### 3. **Self-Contained Object Pattern** (EXCELLENT)
**Location:** `include/actuator.h`, `src/actuator.cpp`

```cpp
class Actuator {
    void setOn();
    void setOff();
    void setBlinking();
    void update();  // MUST be called explicitly by client
};

// In main loop:
sireneActuator.update();
lampActuator.update();
```

**Why Good:**
- ✅ Explicit `update()` call makes behavior visible in `loop()`
- ✅ No implicit state transitions or hidden side effects
- ✅ Self-contained blinking logic (timing, transitions)
- ✅ Prevents "magic" behavior that's hard to debug
- ✅ Clear API: `setBlinking()` → state, `update()` → behavior

**Score:** 10/10 - Recently refactored from bad pattern, now excellent

---

### 4. **Dependency Injection** (GOOD)
**Location:** `include/inputhandler.h`

```cpp
class Inputhandler {
    Inputhandler(Input &setInput, Input &resetInput, 
                Input &manualSet, Input &manualReset);
};

// Usage in main.cpp:
Inputhandler myHandler(setInput, resetInput, manualSet, manualReSet);
```

**Why Good:**
- ✅ Loose coupling: Handler doesn't create its own dependencies
- ✅ Testable: Can inject mock Input objects
- ✅ Flexible: Can work with any Input subclass
- ✅ Inverts control flow properly

**Score:** 8/10 - Good implementation, but inputs are still globals (see Bad Patterns)

---

### 5. **Const-Correctness** (GOOD)
**Location:** `include/input.h`, usage throughout

```cpp
class Input {
    uint8_t getStatus() const;
    uint8_t getPin() const;
};

class Inputhandler {
    const Input &set;
    const Input &reset;
};
```

**Why Good:**
- ✅ Expresses intent: these references won't be modified
- ✅ Prevents accidental mutations
- ✅ Self-documents API contracts
- ✅ Enables compiler checks

**Score:** 8/10 - Well-applied, though const method with side effects exists

---

### 6. **Enumerated Types for Events/States** (GOOD)
**Location:** `include/input.h`, `include/inputhandler.h`

```cpp
enum State { INACTIVE, DEBOUNCING, ACTIVE, DEBOUNCINGLOW };
enum Event { NONE, MANUALRESET, MANUALSET, SETSTARTED, SETSTOPPED, RESET };
```

**Why Good:**
- ✅ Type-safe: can't accidentally pass wrong values
- ✅ Self-documenting: clear enumeration of all valid states/events
- ✅ Compiler catches typos and missing cases
- ✅ Better than magic constants or strings

**Score:** 9/10 - Excellent, but RISING/FALLING events should be qualified clearly

---

### 7. **Named Constants for Timing** (GOOD)
**Location:** `src/inputhandler.cpp` (recently refactored)

```cpp
constexpr uint16_t beeponofftime = 500;
constexpr uint16_t beeptime = 2000;
constexpr uint16_t lampConstantonOnTime = 1000;
constexpr uint32_t sireneConstantonOffTime = 60000;
```

**Why Good:**
- ✅ Magic numbers eliminated
- ✅ Named constants express intent
- ✅ Easy to adjust behavior
- ✅ Comments explain purpose
- ✅ Compile-time constant: zero runtime overhead

**Score:** 9/10 - Excellent, though could be moved to header for reuse

---

### 8. **Reference Parameters for Composite Objects** (GOOD)
**Location:** `include/inputhandler.h`

```cpp
Inputhandler(Input &setInput, Input &resetInput, ...);
```

**Why Good:**
- ✅ No copying of large Input objects
- ✅ Ensures single responsibility for Input
- ✅ Memory efficient on embedded platform
- ✅ Shared ownership model is clear

**Score:** 8/10 - Good, though could use `std::reference_wrapper` for consistency

---

## ❌ BAD / PROBLEMATIC PATTERNS

### 1. **Global Variables** (SEVERE)
**Location:** `main.cpp`, `include/actuator.h`

```cpp
// main.cpp
AnalogInput setInput(...);
AnalogInput resetInput(...);
DigitalInput manualSet(...);
DigitalInput manualReSet(...);
Inputhandler myHandler(...);

// include/actuator.h
extern Actuator sireneActuator;
extern Actuator lampActuator;
```

**Why Bad:**
- ❌ **Hidden Dependencies:** Objects depend on globals without declaring it
- ❌ **Testing Nightmare:** Can't isolate components, no way to reset state between tests
- ❌ **Race Conditions:** In multi-threaded context (not current, but weak foundation)
- ❌ **Implicit Initialization Order:** Constructor order matters, not obvious
- ❌ **Tight Coupling:** `main.cpp` is composition root AND orchestrator
- ❌ **Monolithic:** Hard to extract pieces into library

**Impact:** Can't unit test `Inputhandler` without creating all Input objects

**Fix Options:**
```cpp
// Option 1: Dependency Injection Container
class AppContext {
    AnalogInput setInput;
    AnalogInput resetInput;
    DigitalInput manualSet;
    DigitalInput manualReset;
    Inputhandler handler;
    Actuator sireneActuator;
    Actuator lampActuator;
public:
    void runLoop() { /* orchestrate */ }
};

// Option 2: Singleton (less bad than naked globals)
class Hardware {
    static Hardware& instance();
    Input& getSetInput();
    Actuator& getSireneActuator();
};
```

**Score:** 1/10 - Major code smell, anti-pattern

---

### 2. **Side Effects in Const Method** (MODERATE)
**Location:** `include/input.h`

```cpp
uint8_t getStatus() const {
    const auto retval = (uint8_t)event;
    event = 0;  // ❌ Modifies mutable member in const method!
    return (retval);
}
```

**Why Bad:**
- ❌ **Violates Const Contract:** Const methods shouldn't have side effects
- ❌ **"Call Once" Bug:** Must remember to call exactly once or lose events
- ❌ **Confusing API:** `getStatus()` both reads AND clears state
- ❌ **Not Thread-Safe:** No synchronization around edge case modification

**Real Issue:** What if you check status twice?
```cpp
if (setInput.getStatus() == RISING) {
    handleSet();
}
if (setInput.getStatus() == RISING) {  // ❌ Returns 0, missed event!
    handleAnotherSet();
}
```

**Fix Options:**
```cpp
// Option 1: Separate methods
enum Event getLatestEvent() const;      // Read-only
void clearEvent();                      // Write-only

// Option 2: Remove const
uint8_t getAndClearStatus();           // Honest about side effect

// Option 3: Observer Pattern
void subscribe(EventListener &listener);  // Push instead of pull
```

**Score:** 3/10 - Works in practice, but violates C++ principles

---

### 3. **Magic Numbers in main.cpp** (MODERATE)
**Location:** `src/main.cpp`

```cpp
constexpr uint8_t sensorPinAR = A0;
constexpr uint16_t threshold = 80;
constexpr uint8_t nrHighPolls = 50;
constexpr uint16_t inactiveTime = 10;
constexpr uint8_t timeBetweenPolls = 5;
// ... many more hardcoded values
```

**Why Bad:**
- ❌ **Scattered Configuration:** Tuning requires editing main.cpp
- ❌ **No Central Reference:** Hard to understand all parameters at once
- ❌ **Coupling:** main.cpp has deep knowledge of Input implementation details
- ❌ **Copy-Paste Risk:** Same parameters repeated for audio/digital inputs prone to errors

**Impact:** Want to change audio threshold? Must edit main.cpp and rebuild.

**Fix Options:**
```cpp
// Option 1: Configuration struct
struct InputConfig {
    uint8_t pin;
    uint16_t threshold, thresholdLow;
    uint16_t polltime;
    uint8_t nrHighPolls;
    uint16_t inactiveTime;
};

constexpr InputConfig AUDIO_CONFIG = {A0, 80, 40, 5, 50, 10};
constexpr InputConfig DIGITAL_CONFIG = {3, 1, 0, 5, 10, 10};

setInput = AnalogInput(AUDIO_CONFIG);

// Option 2: EEPROM-based config (advanced)
class ConfigManager { /* load/save from EEPROM */ };
```

**Score:** 4/10 - Functional but not maintainable for parameter tweaking

---

### 4. **Hardcoded Pin Numbers** (MODERATE)
**Location:** Multiple files

```cpp
// main.cpp
constexpr uint8_t sensorPinAR = A0;
constexpr uint8_t sensorPinAL = A1;
constexpr uint8_t pinSet = 3;
constexpr uint8_t pinReset = 4;

// actuator.cpp
Actuator sireneActuator(6, ...);
Actuator lampActuator(13, ...);
```

**Why Bad:**
- ❌ **Hardware Assumptions:** Tightly coupled to specific Arduino pin layout
- ❌ **Not Portable:** Changing board requires code edits everywhere
- ❌ **Scattered:** Pin definitions spread across 3 files
- ❌ **Silent Failures:** No validation that pins exist on target board

**Impact:** Want to use different Arduino model? Audit entire codebase for pin numbers.

**Fix:**
```cpp
// config.h - Single source of truth
namespace PinConfig {
    constexpr uint8_t AUDIO_IN_RIGHT = A0;
    constexpr uint8_t AUDIO_IN_LEFT = A1;
    constexpr uint8_t MANUAL_SET = 3;
    constexpr uint8_t MANUAL_RESET = 4;
    constexpr uint8_t SIRENE = 6;
    constexpr uint8_t LAMP = 13;
}
```

**Score:** 5/10 - Works, but not portable

---

### 5. **Blocking delay() in setup()** (MODERATE)
**Location:** `src/main.cpp`

```cpp
void setup() {
    Serial.begin(9600);
    delay(35000);  // ❌ Block for 35 SECONDS!
    setInput.init();
    // ...
}
```

**Why Bad:**
- ❌ **Blocks Execution:** Nothing happens for 35 seconds
- ❌ **No Feedback:** User doesn't know what's happening
- ❌ **Inflexible:** Can't implement graceful shutdown during boot
- ❌ **Violates Embedded Best Practice:** Should use non-blocking timing
- ✓ **Necessary Evil:** Probably needed for capacitor charging on audio input

**Fix Options:**
```cpp
// Option 1: Non-blocking timing
uint32_t bootStartTime = millis();
bool bootComplete = false;

void setup() {
    Serial.begin(9600);
    bootStartTime = millis();
    Serial.println("Booting...");
}

void loop() {
    if (!bootComplete && millis() - bootStartTime >= 35000) {
        setInput.init();
        resetInput.init();
        // ...
        bootComplete = true;
        Serial.println("Boot complete");
    }
    // ... rest of logic
}

// Option 2: Fast-boot mode with auto-calibration
// Skip initial delay, calibrate on first actual signal
```

**Score:** 3/10 - Functional but poor user experience

---

### 6. **No Input Validation** (MODERATE)
**Location:** Throughout

```cpp
Input::Input(uint8_t pinnr, uint16_t threshold, uint16_t thresholdLow, ...) {
    // ❌ No validation! What if:
    // - pinnr is invalid?
    // - threshold < thresholdLow (inverted)?
    // - threshold == thresholdLow (deadzone)?
    // - polltime == 0 (instant polling)?
    // - counts == 0 (no debouncing)?
}
```

**Why Bad:**
- ❌ **Silent Failures:** Bad parameters don't cause errors, just wrong behavior
- ❌ **Hard to Debug:** May work in some situations, fail in others
- ❌ **No Feedback:** No way to know configuration is wrong
- ❌ **User Error Prone:** Easy to pass parameters in wrong order

**Fix:**
```cpp
class Input {
public:
    Input(...) {
        if (threshold <= thresholdLow) {
            Serial.println("ERROR: threshold must be > thresholdLow");
            while(1); // Safe failure
        }
        if (polltime == 0) {
            Serial.println("ERROR: polltime must be > 0");
            while(1);
        }
        if (counts == 0) {
            Serial.println("WARNING: counts == 0 disables debouncing");
        }
    }
};
```

**Score:** 4/10 - Risky but common in embedded code

---

### 7. **Mixed Concerns in Inputhandler** (MINOR)
**Location:** `include/inputhandler.h`, `src/inputhandler.cpp`

```cpp
class Inputhandler {
    // Manages 4 inputs, state machine logic, event routing
    // Doesn't directly control actuators but knows about them
};
```

**Why Slightly Bad:**
- ⚠️ **Growing Responsibility:** Currently managing state + 4 input sources
- ⚠️ **Hard to Extend:** Adding new inputs requires modifying Inputhandler
- ⚠️ **No Abstraction:** Hardcoded to exactly 4 inputs

**Not Terrible Because:**
- ✅ Single state machine is coherent
- ✅ Not actually controlling outputs (clean separation)
- ✅ For this specific use case (2 audio + 2 button), fixed

**Score:** 6/10 - Acceptable for current scope, but limits flexibility

---

### 8. **Excessive Serial Logging** (MINOR - PARTIALLY FIXED)
**Location:** `src/input.cpp`, `src/actuator.cpp` (was worse)

```cpp
void Input::poll() {
    // ...
    Serial.print(val);
    Serial.print(" : ");
    Serial.print("pin high :  ");
    Serial.println(pin);  // ❌ Called every 5ms when level high!
}
```

**Why Bad:**
- ❌ **Performance:** Serial I/O is slow, called at high frequency
- ⚠️ **Debug Spam:** Output hard to read with 200 lines/sec
- ❌ **Masking Issues:** Real errors buried in noise

**Status:** ✅ MOSTLY FIXED - Reduced logging in recent changes

**Score:** 7/10 - Much better, some debug traces remain for development

---

### 9. **No Observer/Event Pattern** (MINOR)
**Location:** Push vs Pull decision in main loop

```cpp
// Current: Pull model
void loop() {
    setInput.poll();
    resetInput.poll();
    myHandler.handle();  // myHandler pulls from inputs
    // ...
}
```

**Alternative: Observer model (not required)**
```cpp
// Push model - more responsive
class EventListener {
    virtual void onRising() = 0;
    virtual void onFalling() = 0;
};

class Input {
    void subscribe(EventListener &listener);
    // Calls listener methods when events occur
};
```

**Assessment:**
- ✅ **Current Model OK:** Pull-based polling fine for embedded
- ⚠️ **Not Wrong:** Just different trade-offs
- ❌ **Would Add Complexity:** Not worth it for this application

**Score:** 8/10 - Current choice is reasonable

---

### 10. **Missing Protected Members** (MINOR)
**Location:** `include/input.h`

```cpp
class Input {
private:  // ❌ Subclasses can't access
    enum State { INACTIVE, DEBOUNCING, ACTIVE, DEBOUNCINGLOW };
    uint16_t threshold;
    uint16_t thresholdLow;
    // ...
};

class AnalogInput : public Input {
    // Can't access threshold, thresholdLow directly
    // Must use public getter
};
```

**Why Minor Issue:**
- ⚠️ **Encapsulation is Good:** prevents misuse
- ⚠️ **Could Be Better:** Protected instead of private allows subclass optimization
- ✅ **Getters Work:** Current approach is safe and correct

**Score:** 7/10 - Conservative approach, not wrong

---

## SUMMARY TABLE

| Pattern | Category | Score | Status |
|---------|----------|-------|--------|
| Template Method (Input) | ✅ Architecture | 10/10 | Excellent |
| State Machines | ✅ Architecture | 9/10 | Excellent |
| Actuator Self-Contained | ✅ Design | 10/10 | Excellent |
| Dependency Injection | ✅ Design | 8/10 | Good |
| Const-Correctness | ✅ C++ Practice | 8/10 | Good |
| Enum Types | ✅ Naming | 9/10 | Good |
| Named Constants (timing) | ✅ Naming | 9/10 | Good |
| Reference Parameters | ✅ Design | 8/10 | Good |
| **Global Variables** | ❌ Anti-Pattern | 1/10 | SEVERE |
| **Const Side-Effects** | ❌ C++ Violation | 3/10 | MODERATE |
| **Magic Numbers (config)** | ⚠️ Maintainability | 4/10 | MODERATE |
| **Hardcoded Pins** | ⚠️ Portability | 5/10 | MODERATE |
| **Blocking delay()** | ⚠️ UX | 3/10 | MODERATE |
| No Input Validation | ⚠️ Robustness | 4/10 | MODERATE |
| Mixed Concerns | ⚠️ Scalability | 6/10 | MINOR |
| Serial Logging | ⚠️ Performance | 7/10 | MINOR (Improving) |
| No Observer Pattern | ℹ️ Alternative | 8/10 | N/A |
| Private Inheritance Access | ℹ️ Encapsulation | 7/10 | MINOR |

---

## OVERALL ASSESSMENT

**Strengths:**
- ✅ **Template Method** - Excellent reuse of debouncing algorithm
- ✅ **State Machines** - Clear, maintainable control flow  
- ✅ **Object Design** - Self-contained Actuator prevents hidden behavior
- ✅ **Type Safety** - Enums and const-correctness used well
- ✅ **Separation of Concerns** - Input parsing, debouncing, control logic cleanly separated

**Weaknesses:**
- ❌ **Global Variables** - Biggest problem, limits testability and reusability
- ❌ **Configuration** - Scattered throughout main.cpp, hard to tune
- ❌ **Extensibility** - Hardcoded to specific pin/sensor layout
- ⚠️ **Robustness** - No input validation or error handling
- ⚠️ **User Experience** - Long boot delay, cryptic serial output

---

## RECOMMENDATIONS (Priority Order)

### CRITICAL (Fix First)
1. **Extract Configuration**
   - Create `config.h` with all pins and timing constants
   - Use single namespace or struct for parameter groups
   - Effort: 1 hour | Impact: High

2. **Refactor Global Variables**
   - Create `Hardware` class or dependency container
   - Inject dependencies into objects
   - Effort: 2 hours | Impact: Very High (enables testing)

### HIGH (Should Fix)
3. **Fix Const/Side-Effect Issue**
   - Separate `getLatestEvent()` (const) from `clearEvent()` (or make non-const)
   - Effort: 30 min | Impact: Moderate

4. **Add Input Validation**
   - Validate pin numbers, threshold ordering, timing parameters
   - Graceful failure with Serial error messages
   - Effort: 1 hour | Impact: Moderate

### MEDIUM (Nice to Have)
5. **Non-Blocking Boot Sequence**
   - Replace `delay(35000)` with state machine
   - Provide user feedback ("Booting...", "Ready")
   - Effort: 1.5 hours | Impact: UX improvement

6. **Extract Timing Configuration**
   - Move constexpr to header file for reuse
   - Create timing constants in central location
   - Effort: 30 min | Impact: Maintainability

### LOW (Polish)
7. **Remove Debug Serial Output**
   - Conditional compilation for debug vs production
   - Use proper logging library if Serial usage grows
   - Effort: 30 min | Impact: Low

---

## CONCLUSION

RVBAlarm demonstrates **good architectural thinking** with excellent use of Template Method and State Machine patterns. The **Actuator class refactoring** shows learning and improvement.

However, **global variables** undermine the entire foundation, and **scattered configuration** makes tuning difficult. These are fixable issues that would significantly improve code quality.

**Rating: 6.5/10** - Solid embedded system, with room for improvement in testability and maintainability.
