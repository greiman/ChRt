#if defined(__SAMD21G18A__) || defined(__SAMD21J18A__)
#include "../hal/osal.h"
#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
#if OSAL_ST_FREQUENCY != 1000
#error "SAMD only supports CH_CFG_ST_FREQUENCY == 1000"
#endif

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
#endif  // OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
#endif  // #if defined(__SAMD21G18A__) || defined(__SAMD21J18A__)