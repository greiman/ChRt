#if defined(__SAM3X8E__) || defined(__SAM3X8H__)
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
#endif  // defined(__SAM3X8E__) || defined(__SAM3X8H__)