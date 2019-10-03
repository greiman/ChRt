// Connect a scope to pin 13
// Measure difference in time between first pulse with no context switch
// and second pulse started in thread 2 and ended in thread 1.
// Difference should be about 15-16 usec on a 16 MHz 328 Arduino.
// Under a microsecond on a Teensy 3.6.
// About 200 ns on Teensy 4.0
#include "ChRt.h"

// LED_BUILTIN pin on Arduino is usually pin 13.

// Semaphore to trigger context switch
SEMAPHORE_DECL(sem, 0);
//------------------------------------------------------------------------------
// thread 1 - high priority thread to set pin low.
// 64 byte stack beyond task switch and interrupt needs.
static THD_WORKING_AREA(waThread1, 64);

static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  while (true) {
    chSemWait(&sem);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
//------------------------------------------------------------------------------
// thread 2 - lower priority thread to toggle LED and trigger thread 1.
// 64 byte stack beyond task switch and interrupt needs.
static THD_WORKING_AREA(waThread2, 64);

static THD_FUNCTION(Thread2, arg) {
  (void)arg;
  pinMode(LED_BUILTIN, OUTPUT);
  while (true) {
    // First pulse to get time with no context switch.
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    // Start second pulse.
    digitalWrite(LED_BUILTIN, HIGH);
    // Trigger context switch for task that ends pulse.
    chSemSignal(&sem);
    // Sleep until next tick.
    chThdSleep(1);
  }
}
//------------------------------------------------------------------------------
void chSetup() {
  // Start high priority thread.
  chThdCreateStatic(waThread1, sizeof(waThread1),
    NORMALPRIO + 2, Thread1, NULL);

  // Start lower priority thread.
  chThdCreateStatic(waThread2, sizeof(waThread2),
    NORMALPRIO+1, Thread2, NULL);
}
//------------------------------------------------------------------------------
void setup() {
  chBegin(&chSetup);
  // chBegin() resets stacks and should never return.
  while (true) {}  
}
//------------------------------------------------------------------------------
void loop() {
  // Not used.
}