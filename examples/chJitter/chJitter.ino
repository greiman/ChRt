// Test of jitter in sleep for one tick.
//
// Idle main thread prints min and max time between sleep calls.
//
// Note: access to shared variables tmin and tmax is not atomic
// so glitches are possible if context switch happens during main loop
// access to these variables.
//
#include "ChRt.h"

volatile  uint16_t tmax = 0;
volatile  uint16_t tmin = 0XFFFF;
//------------------------------------------------------------------------------
// Thread for sleep interval.
static THD_WORKING_AREA(waThread1, 64);

static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  // Initialize tlast.
  chThdSleep(1);
  uint32_t tlast = micros();
  
  while (true) {
  
    // Sleep until next tick.
    chThdSleep(1);
    
    // Get wake time.
    uint32_t tmp = micros();
    
    // Calculate min and max interval between wake times.
    uint16_t diff = tmp - tlast;
    if (diff < tmin) tmin = diff;
    if (diff > tmax) tmax = diff;
    tlast = tmp;
  }
}
//------------------------------------------------------------------------------
void chStatup() {
  // Start higher priority thread.
  chThdCreateStatic(waThread1, sizeof(waThread1), 
    NORMALPRIO + 1, Thread1, NULL);
}    
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial.
  while (!Serial) {}
  
  // Start ChibiOS.
  chBegin(chStatup);
  // chBegin() resets stacks and should never return.
  while (true) {}   
}
//------------------------------------------------------------------------------
void loop() {
  int np = 10;

  while (true) {
    chThdSleepMilliseconds(1000);
    Serial.print(tmin);
    Serial.write(',');
    Serial.println(tmax);
    if (np++ == 10) {
      np = 0;
      tmin = 0XFFFF;
      tmax = 0;
      Serial.println("clear");
    }
  }
}