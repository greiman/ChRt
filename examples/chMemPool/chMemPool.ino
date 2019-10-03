// Example of memory pool and mailboxes with two senders and one receiver.
#include "ChRt.h"
//------------------------------------------------------------------------------
// Mailbox/memory pool object count.
const size_t MB_COUNT = 6;

// Type for a memory pool object.
struct PoolObject_t {
  char* name;
  int msg;
};
// Array of memory pool objects.
PoolObject_t PoolObject[MB_COUNT];

// Memory pool structure.
MEMORYPOOL_DECL(memPool, sizeof(PoolObject_t),PORT_NATURAL_ALIGN, NULL);
//------------------------------------------------------------------------------
// Slots for mailbox messages.  Will hold pointers to pool objects.
msg_t letter[MB_COUNT];

// Mailbox queue structure.
MAILBOX_DECL(mail, &letter, MB_COUNT);
//------------------------------------------------------------------------------
// Data structures and stack for thread 2.
static THD_WORKING_AREA(waTh2, 200);

// Data structures and stack for thread 3.
static THD_WORKING_AREA(waTh3, 200);
//------------------------------------------------------------------------------
// Send message every 1000 ms.
static THD_FUNCTION(thdFcn, name) {
  int msg = 0;

  while (true) {

    // Get object from memory pool.
    PoolObject_t* p = (PoolObject_t*)chPoolAlloc(&memPool);
    if (!p) {
      Serial.println("chPoolAlloc failed");
      while (true);
    }
    // Form message.
    p->name = (char*)name;
    p->msg = msg++;

    // Send message.
    msg_t s = chMBPostTimeout(&mail, (msg_t)p, TIME_IMMEDIATE);
    if (s != MSG_OK) {
      Serial.println("chMBPost failed");
      while (true);  
    }
    chThdSleepMilliseconds(1000);    
  }
}
//------------------------------------------------------------------------------
void chSetup() {
  // Fill pool with PoolObject array.
  chPoolLoadArray(&memPool, PoolObject, MB_COUNT);  

  // Schedule thread 2.
  chThdCreateStatic(waTh2, sizeof(waTh2), NORMALPRIO, thdFcn, (void*)"Th 2");

  // Schedule thread 3.
  chThdCreateStatic(waTh3, sizeof(waTh2), NORMALPRIO, thdFcn, (void*)"Th 3");
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
// loop() is receiver thread.
void loop() {
    PoolObject_t *p = 0;;

    // Get mail.
    chMBFetchTimeout(&mail, (msg_t*)&p, TIME_INFINITE);

    Serial.print(p->name);
    Serial.write(' ');
    Serial.println(p->msg);

    // Put memory back into pool.
    chPoolFree(&memPool, p);
}