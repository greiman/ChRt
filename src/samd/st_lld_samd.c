#if defined(__SAMD21G18A__) || defined(__SAMD21J18A__)
#include "../rt/ch.h"

static int sysTickEnabled = 0;

int sysTickHook(void) { 
  if (sysTickEnabled) {
  CH_IRQ_PROLOGUE();

  chSysLockFromISR();
  chSysTimerHandlerI();
  chSysUnlockFromISR();

  CH_IRQ_EPILOGUE();
  }
  return 0;
}
void st_lld_init(void) {
  sysTickEnabled = 1;  
}
#endif  // #if defined(__SAMD21G18A__) || defined(__SAMD21J18A__)