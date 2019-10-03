// Demonstration of simple events where one thread signals another.
// ChibiOS supports a more general model of event sources and event listeners.
#include "ChRt.h"

// Set PRINT_UNUSED_STACK nonzero to check stack use.
#define PRINT_UNUSED_STACK 0
//------------------------------------------------------------------------------
thread_t* listenThread;
thread_t* thread2;
thread_t* thread3;
//------------------------------------------------------------------------------
// Declare a stack with 200 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread1, 200);

// Declare thread function for thread 1.
THD_FUNCTION(Thread1, arg) {
  (void)arg;
  
  listenThread = chThdGetSelfX();
  while (TRUE) {
    eventmask_t m = chEvtWaitAnyTimeout(ALL_EVENTS, TIME_INFINITE);
    if (m & EVENT_MASK(0)) {
      // Tell Thread2 event was processed.
      chEvtSignal(thread2, EVENT_MASK(0));
      Serial.println("Signal from Thread2");
    }
    if (m & EVENT_MASK(1)) {
      // Tell Thread3 event was processed.      
      chEvtSignal(thread3, EVENT_MASK(1));
      Serial.println("Signal from Thread3");
    }
#if PRINT_UNUSED_STACK
    printUnusedStack();
#endif
  }
}
//------------------------------------------------------------------------------
// Declare a stack with 200 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread2, 200);

// Declare thread function for thread 2.
THD_FUNCTION(Thread2, arg) {
  (void)arg;
  
  thread2 = chThdGetSelfX();
  while (TRUE) {
    // Use event ID 0 for this thread.   
    chEvtSignal(listenThread, EVENT_MASK(0));
    
    // Wait for signal from listener.
    eventmask_t m = chEvtWaitAnyTimeout(ALL_EVENTS, 1000);
    if (m != EVENT_MASK(0)) {
      Serial.print("Thread2 err, m: ");
      Serial.println(m);
      break;
    }
    chThdSleepMilliseconds(1100);
  }
  chThdSleep(TIME_INFINITE);
}
//------------------------------------------------------------------------------
// Declare a stack with 200 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread3, 200);

// Declare thread function for thread 3.
THD_FUNCTION(Thread3, arg) {
  (void)arg;
  thread3 = chThdGetSelfX();
  while (TRUE) {
    // Use event ID 1 for this thread.
    chEvtSignal(listenThread, EVENT_MASK(1));
    
    // Wait for signal from listener.    
    eventmask_t m = chEvtWaitAnyTimeout(ALL_EVENTS, 1000);
    if (m != EVENT_MASK(1)) {
      Serial.print("Thread3 err, m: ");
      Serial.println(m);
      break;
    }
    chThdSleepMilliseconds(1000);
  }
  chThdSleep(TIME_INFINITE);  
}
//------------------------------------------------------------------------------
void printUnusedStack() {
  Serial.print(F("Unused Stack: "));
  Serial.print(chUnusedThreadStack(waThread1, sizeof(waThread1)));
  Serial.print(' ');
  Serial.print(chUnusedThreadStack(waThread2, sizeof(waThread2)));
  Serial.print(' ');
  Serial.print(chUnusedThreadStack(waThread3, sizeof(waThread3)));
  Serial.print(' ');
  Serial.print(chUnusedMainStack());
#ifdef __arm__
  // ARM has separate handler stack for ISR interrupts. 
  Serial.print(' ');
  Serial.print(chUnusedHandlerStack());
#endif  // __arm__
  Serial.println();
}
//------------------------------------------------------------------------------
// Continue setup() after chBegin().
void chSetup() {
  // Start threads.  Order is important, listener must be first.
  chThdCreateStatic(waThread1, sizeof(waThread1),
    NORMALPRIO + 2, Thread1, NULL);

  chThdCreateStatic(waThread2, sizeof(waThread2),
    NORMALPRIO + 1, Thread2, NULL);

  chThdCreateStatic(waThread3, sizeof(waThread3),
    NORMALPRIO + 1, Thread3, NULL);    
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
void loop() {
}