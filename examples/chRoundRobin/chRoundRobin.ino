// Demo runing two threads round-robin.
//
// Warning, this example uses wait loops to illustrate round-robin.
// Use chThdSleep in normal applications.
//
// Both threads use wait loops.  Each thread will execute
// for one quantum then be preempted.
//
// Run with the blink thread at NORMALPRIO + 1 and NORMALPRIO - 1 to
// See blockage due to use of wait loops.
//
#include "ChRt.h"
//------------------------------------------------------------------------------
// 32 byte stack beyond task switch and interrupt needs.
static THD_WORKING_AREA(waBlink, 64);

static THD_FUNCTION(blink, arg) {
  // blink twice per second
  pinMode(LED_BUILTIN, OUTPUT);
  uint32_t next = millis();
  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    next += 100;
    while ((int32_t)(millis() - next) < 0) {}
    digitalWrite(LED_BUILTIN, LOW);
    next += 400;
    while ((int32_t)(millis() - next) < 0) {}
  }
}
//------------------------------------------------------------------------------
void chStartup() {
  if (CH_CFG_TIME_QUANTUM == 0) {
    Serial.println("CH_CFG_TIME_QUANTUM must be nonzero for round-robin.");
    while (true) {}
  }
  // Start blink thread.
  chThdCreateStatic(waBlink, sizeof(waBlink), NORMALPRIO, blink, NULL); 
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial.
  while (!Serial) {}
  
  // Initialize and start ChibiOS.
  chBegin(chStartup);
  // chBegin() resets stacks and should never return.
  while (true) {}
}
//------------------------------------------------------------------------------
void loop() {
  uint32_t next = 0;
  while (true) {
    next += 1000;
    while ((int32_t)(millis() - next) < 0) {}
    Serial.println(millis());
  }
}
