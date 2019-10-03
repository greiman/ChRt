#if defined(__SAM3X8E__) || defined(__SAM3X8H__)
#include "../hal/osal.h"
#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
#if OSAL_ST_FREQUENCY != 1000
#error "Due only supports CH_CFG_ST_FREQUENCY == 1000"
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
#endif  // defined(__SAM3X8E__) || defined(__SAM3X8H__)