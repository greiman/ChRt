#include "Arduino.h"

#if defined(__MK20DX256__) || defined(__MK20DX128__)\
    || defined(__MK64FX512__) || defined(__MK66FX1M0__)
    
#include "../hal/osal.h"

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
#if 1000000 % OSAL_ST_FREQUENCY != 0
#error "CH_CFG_ST_FREQUENCY is not obtainable due to integer rounding"
#endif
#if OSAL_ST_FREQUENCY > 10000
#error "CH_CFG_ST_FREQUENCY is too high"
#endif
//------------------------------------------------------------------------------
static void tick() {
  CH_IRQ_PROLOGUE();
  chSysLockFromISR();
  chSysTimerHandlerI();
  chSysUnlockFromISR();
  CH_IRQ_EPILOGUE();
}
//------------------------------------------------------------------------------
IntervalTimer chTimer;
extern "C" void st_lld_init(void) {
  chTimer.begin(tick, 1000000/OSAL_ST_FREQUENCY);
}
#endif  // OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
#endif  // defined(__MK20DX256__) || defined(__MK20DX128__) ...