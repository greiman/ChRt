// This example illustrates cooperative scheduling. Cooperative scheduling
// simplifies multitasking since no preemptive context switches occur.
//
// You must call chYield() or other ChibiOS functions such as chThdSleep
// to force a context switch to other threads.
//
// Insert a delay(100) in loop() to see the effect of not
// using chThdSleep with cooperative scheduling.
//
// Setting CH_TIME_QUANTUM to zero disables the preemption for threads
// with equal priority and the round robin becomes cooperative.
// Note that higher priority threads can still preempt, the kernel
// is always preemptive.
//
#include "ChRt.h"
// LED_BUILTIN pin on Arduino is usually pin 13.

volatile uint32_t count = 0;
volatile uint32_t maxDelay = 0;
//------------------------------------------------------------------------------
// thread 1 blink LED
// 64 byte stack beyond task switch and interrupt needs.
static THD_WORKING_AREA(waThread1, 64);

static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  pinMode(LED_BUILTIN, OUTPUT);
  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    chThdSleepMilliseconds(50);
    digitalWrite(LED_BUILTIN, LOW);
    chThdSleepMilliseconds(150);
  }
}
//------------------------------------------------------------------------------
// thread 2 increment a counter and records max delay.
// 64 byte stack beyond task switch and interrupt needs.
static THD_WORKING_AREA(waThread2, 64);

static THD_FUNCTION(Thread2, arg) {
  (void)arg;
  while (true) {
    count++;
    uint32_t t = micros();
    // Yield so other threads can run.
    chThdYield();
    t = micros() - t;
    if (t > maxDelay) maxDelay = t;
  }
}
//------------------------------------------------------------------------------
// Continue setup() after chBegin().
void chSetup() {
  if (CH_CFG_TIME_QUANTUM) {
    Serial.println("You must set CH_CFG_TIME_QUANTUM zero in");
#if defined(__arm__)
    Serial.print("src/<board type>/chconfig<board>.h");
#elif defined(__AVR__)
    Serial.print(F("src/avr/chconfig_avr.h")); 
#endif 
    Serial.println(F(" to enable cooperative scheduling."));
    while (true) {}
  }  
  chThdCreateStatic(waThread1, sizeof(waThread1),
    NORMALPRIO, Thread1, NULL);

  chThdCreateStatic(waThread2, sizeof(waThread2),
    NORMALPRIO, Thread2, NULL);
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial.
  while (!Serial) {}

  // Start ChibiOS.
  chBegin(chSetup);
  // chBegin() resets stacks and should never return.
  while (true) {}  
}
//------------------------------------------------------------------------------
void loop() {
  Serial.println("Count, MaxDelay micros");  
  while (true) {
    Serial.print(count);
    Serial.write(',');
    Serial.println(maxDelay);
    count = 0;
    maxDelay = 0;
    // Enable next line to see blocking of above threads.   
//    delay(100);
    // Allow other threads to run for 1 sec.
    chThdSleepMilliseconds(1000);
  }
}