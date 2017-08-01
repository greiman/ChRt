// Simple demo of three threads.
// LED blink thread, count thread, and main thread.
#include "ChRt.h"

// LED_BUILTIN pin on Arduino is usually pin 13.

volatile uint32_t count = 0;
//------------------------------------------------------------------------------
// thread 1 - high priority for blinking LED.
// 64 byte stack beyond task switch and interrupt needs.
static THD_WORKING_AREA(waThread1, 64);

static THD_FUNCTION(Thread1 , arg) {
  (void)arg;

  pinMode(LED_BUILTIN, OUTPUT);

  // Flash led every 200 ms.
  while (true) {
    // Turn LED on.
    digitalWrite(LED_BUILTIN, HIGH);

    // Sleep for 50 milliseconds.
    chThdSleepMilliseconds(50);

    // Turn LED off.
    digitalWrite(LED_BUILTIN, LOW);

    // Sleep for 150 milliseconds.
    chThdSleepMilliseconds(150);
  }
}
//------------------------------------------------------------------------------
// thread 2 - count when higher priority threads sleep.
// 64 byte stack beyond task switch and interrupt needs.
static THD_WORKING_AREA(waThread2, 64);

static THD_FUNCTION(Thread2, arg) {
  (void)arg;

  while (true) {
    noInterrupts();
    count++;
    interrupts();
  }
}
//------------------------------------------------------------------------------
// Name chSetup is not special - must be same as used in chBegin() call.
void chSetup() {
  // Start blink thread. Priority one more than loop().
  chThdCreateStatic(waThread1, sizeof(waThread1),
                    NORMALPRIO + 1, Thread1, NULL);
  // Start count thread.  Priority one less than loop().
  chThdCreateStatic(waThread2, sizeof(waThread2),
                    NORMALPRIO - 1, Thread2, NULL);
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial.
  while (!Serial) {}

  chBegin(chSetup);
  // chBegin() resets stacks and should never return.
  while (true) {}
}
//------------------------------------------------------------------------------
// Runs at NORMALPRIO.
void loop() {
  // Sleep for one second.
  chThdSleepMilliseconds(1000);
  
  // Print count for previous second.
  Serial.print(F("Count: "));
  Serial.print(count);
  
  // Zero count.
  count = 0;

  // Print unused stack space in bytes.
  Serial.print(F(", Unused Stack: "));
  Serial.print(chUnusedThreadStack(waThread1, sizeof(waThread1)));
  Serial.print(' ');
  Serial.print(chUnusedThreadStack(waThread2, sizeof(waThread2)));
  Serial.print(' ');
  Serial.print(chUnusedMainStack());
#ifdef __arm__
  // ARM has separate stack for ISR interrupts. 
  Serial.print(' ');
  Serial.print(chUnusedHandlerStack());
#endif  // __arm__
  Serial.println();
}
