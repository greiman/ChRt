#include "Arduino.h"
#include "ChRt.h"

#ifdef CORE_TEENSY
#define __StackTop _estack
#endif  // CORE_TEENSY

extern char __StackTop;
extern char *__brkval;
extern "C" char *sbrk(int);

#if defined(__arm__)
const uint8_t* handlerStackTop = (uint8_t*)&__StackTop;
const uint8_t* processStackTop = handlerStackTop - HANDLER_STACK_SIZE;
#elif defined(__AVR__)
const uint8_t* processStackTop = (uint8_t*)RAMEND;
#else  // defined(__arm__)
#error CPU type
#endif  // defined(__arm__)
//------------------------------------------------------------------------------
uint8_t* heapEnd() {
  static uint8_t* heap_end = 0;

#ifdef __AVR__
  uint8_t* p = (uint8_t*)(__brkval ? __brkval : __malloc_heap_start);
#else  // __AVR__
  uint8_t* p = (uint8_t*)sbrk(0);
#endif  // __AVR_

  if (heap_end < p && p < processStackTop) {
    heap_end = p;
  }
  return heap_end;
}
//------------------------------------------------------------------------------
uint8_t* stackBase() {
#if defined(__IMXRT1062__)
  extern uint8_t _ebss;
  return &_ebss;
#else  // defined(__IMXRT1062__)
return heapEnd();  
#endif  // defined(__IMXRT1062__)
}
//------------------------------------------------------------------------------
static size_t unusedStackByteCount(const void* bgn, const void* end) {
#if CH_DBG_FILL_THREADS
  uint8_t* b = (uint8_t*)bgn;
  uint8_t* p = b;
   while (p < end && *p == CH_DBG_STACK_FILL_VALUE) {
    p++;
  }
  return p - b;
#else  // CH_DBG_FILL_THREADS
  (void)bgn;
  (void)end;
  return 0;
#endif  // CH_DBG_FILL_THREADS
}
//------------------------------------------------------------------------------
/** continuation of main thread */
static void (*mainFcn)() = 0;

void chBegin(void (*mainThread)()) {
  noInterrupts();
  mainFcn = mainThread;

#if CH_DBG_FILL_THREADS
  // start of stack
  uint8_t* p = stackBase();
  // Fill stack hope compiler allows this.
  while (p < (uint8_t*)(&p - 4)) {
    *p++ = CH_DBG_STACK_FILL_VALUE;
  }
#endif // CH_DBG_FILL_THREADS

#ifdef __arm__
#if defined(CORTEX_USE_FPU) && CORTEX_USE_FPU
  // Select PSP, FPCA - Floating-Point Context Active.
  const uint32_t control = 0X02 | 0X04;
#else // CORTEX_USE_FPU
  // Select PSP.
  const uint32_t control = 0X2; 
#endif // CORTEX_USE_FPU

  // Set Handler Stack Pointer.
  asm volatile ("msr     MSP, %0" : : "r" (handlerStackTop));

  // Set Process Stack Pointer.
  asm volatile ("msr     PSP, %0" : : "r" (processStackTop));

  // Set Control Register
  asm volatile ("msr     CONTROL, %0" : : "r" (control));

  // Instruction Synchronization Barrier
  asm volatile ("isb");
#else  //  __arm__
  // Set Stack Pointer to RAMEND.
  SP = (uint16_t)processStackTop;
#endif  //  __arm__
  // Initialize the System Tick Driver.
  st_lld_init();

  // ChibiOS/RT initialization
  chSysInit();

  // Call continuation of main thread.
  if (mainFcn) {mainFcn();}
  
  // loop() becomes main thread.
  while (true) {loop();}
}
//------------------------------------------------------------------------------
size_t chUnusedHandlerStack() {
#ifdef __arm__
  return unusedStackByteCount(processStackTop, handlerStackTop);
#else  // __arm__
  return 0;
#endif // __arm__
}
//------------------------------------------------------------------------------
size_t chUnusedMainStack() {
  uint8_t* h = stackBase();
  size_t n = 0;
  // Search for unused heap/stack.
  while (&h[n] < processStackTop && n < 16) {
    if (h[n++] != CH_DBG_STACK_FILL_VALUE) {
      h += n;
      n = 0;
    }
  }
  return unusedStackByteCount(h, processStackTop);
}
//------------------------------------------------------------------------------
size_t chUnusedThreadStack(void *wsp, size_t size) {
  uint8_t* stackTop = ((uint8_t *)wsp + size -
                       MEM_ALIGN_NEXT(sizeof(thread_t), PORT_STACK_ALIGN));
  return unusedStackByteCount(wsp, stackTop);
}