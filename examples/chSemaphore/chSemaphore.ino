// Example of counting semaphore.
// Note, one thread may get an unfair share time in the critical section.
#include "ChRt.h"

// Declare and initialize a semaphore for limiting access.
//
// Initialize semSlots to one slot to see sequential behavior.
SEMAPHORE_DECL(semSlots, 2);

// Data structures and stack for thread 2.
static THD_WORKING_AREA(waTh2, 200);

// Data structures and stack for thread 3.
static THD_WORKING_AREA(waTh3, 200);
//------------------------------------------------------------------------------
static THD_FUNCTION(thdFcn, name) {
 while (true) {

  // Wait for slot.
  chSemWait(&semSlots);

    // Only two threads can be in this region at a time.
    Serial.println((char*)name);

    chThdSleepMilliseconds(1000);

    // Exit region.
    chSemSignal(&semSlots);
  }
}
//------------------------------------------------------------------------------
void chStartup() {
  // Schedule thread 2.
  chThdCreateStatic(waTh2, sizeof(waTh2), NORMALPRIO, thdFcn, (void*)"Th 2");

  // Schedule thread 3.
  chThdCreateStatic(waTh3, sizeof(waTh3), NORMALPRIO, thdFcn, (void*)"Th 3");

  // Main thread is thread 1 at NORMALPRIO.
  thdFcn((void*)"Th 1");
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
void loop() {/* not used */}
