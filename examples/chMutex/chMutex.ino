// Example of mutex to prevent print calls from being scrambled.
#include "ChRt.h"

// Set USE_MUTEX to zero for scrambled print.
#define USE_MUTEX 1

// Declare and initialize a mutex for limiting access to threads.
MUTEX_DECL(demoMutex);

// Data structures and stack for thread 2.
static THD_WORKING_AREA(waTh2, 200);

// Data structures and stack for thread 3.
static THD_WORKING_AREA(waTh3, 200);
//------------------------------------------------------------------------------
void notify(const char* name, int state) {

#if USE_MUTEX
  // Wait to enter print region.
  chMtxLock(&demoMutex);
#endif  // USE_MUTEX
  
  // Only one thread in this region while doing prints.
  Serial.print(name);
  chThdSleep(100);
  Serial.write(": ");
  Serial.println(state);
 
#if USE_MUTEX 
  // Exit protected region.
  chMtxUnlock(&demoMutex);
#endif  // USE_MUTEX  
}
//------------------------------------------------------------------------------
static THD_FUNCTION(thdFcn, arg) {
  while (true) {
    notify((const char*)arg, 0);
    chThdSleepMilliseconds(1000);
    notify((const char*)arg, 1);
    chThdSleepMilliseconds(1000);
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
void loop() {/* Not used. */}