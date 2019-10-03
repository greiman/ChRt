// Example use of mailboxes. Use mailbox to return free slots.
#include "ChRt.h"

#define NUM_BUFFERS 4
#define BUFFERS_SIZE 32
 
static char buffers[NUM_BUFFERS][BUFFERS_SIZE];

// Slots for free buffer pointers.
static msg_t free_buffers_queue[NUM_BUFFERS];
// Free queue mailbox structure.
MAILBOX_DECL(free_buffers, &free_buffers_queue, NUM_BUFFERS);

// Slots for filled buffer pointers.
static msg_t filled_buffers_queue[NUM_BUFFERS];
// Filled queue mailbox structure.
MAILBOX_DECL(filled_buffers, &filled_buffers_queue, NUM_BUFFERS);
//------------------------------------------------------------------------------
// Declare a stack with 300 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread1, 300);

// Declare the thread function for thread 1.
THD_FUNCTION(Thread1, arg) {
  (void)arg;
  int n = 0;
  while (true) {
    void *pbuf = nullptr; 
    
    // Waiting for a free buffer.
    msg_t msg = chMBFetchTimeout(&free_buffers, (msg_t *)&pbuf, TIME_INFINITE);
    if (msg != MSG_OK) {
      Serial.println("fetch free failed");
      chThdSleep(TIME_INFINITE);
    }      
    sprintf((char*)pbuf, "message %d", n++);
    (void)chMBPostTimeout(&filled_buffers, (msg_t)pbuf, TIME_INFINITE);
    chThdSleepMilliseconds(1000);    
  }
}
//------------------------------------------------------------------------------
// Declare a stack with 200 bytes beyond context switch and interrupt needs. 
THD_WORKING_AREA(waThread2, 200);

// Declare the thread function for thread 2.
THD_FUNCTION(Thread2, arg) {
  (void)arg;
  while (true) {
    void *pbuf = nullptr;
    // Waiting for a filled buffer.
    msg_t msg = chMBFetchTimeout(&filled_buffers, (msg_t *)&pbuf, TIME_INFINITE);
    if (msg != MSG_OK) {
      Serial.println("fetch filled failed");
      chThdSleep(TIME_INFINITE);
    }  
    Serial.println((char*)pbuf);    
    
    (void)chMBPostTimeout(&free_buffers, (msg_t)pbuf, TIME_INFINITE);
  }  
}
//------------------------------------------------------------------------------
// Continue setup() after chBegin().
void chSetup() {
  
  //Put all buffers in free queue.
  for (int i = 0; i < NUM_BUFFERS; i++) {
   (void)chMBPostTimeout(&free_buffers, (msg_t)&buffers[i], TIME_INFINITE);
  }  
  
  chThdCreateStatic(waThread1, sizeof(waThread1),
    NORMALPRIO + 1, Thread1, NULL);

  chThdCreateStatic(waThread2, sizeof(waThread2),
    NORMALPRIO + 1, Thread2, NULL);
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
void loop() {
}