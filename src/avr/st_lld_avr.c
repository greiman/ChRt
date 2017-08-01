#ifdef  __AVR__
#include "../rt/ch.h"
#if (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) || defined(__DOXYGEN__)

/**
 * @brief Timer handler for periodic mode.
 */
CH_IRQ_HANDLER(TIMER0_COMPA_vect) {
  CH_IRQ_PROLOGUE();

  chSysLockFromISR();
  chSysTimerHandlerI();
  chSysUnlockFromISR();

  CH_IRQ_EPILOGUE();
}
  /*
   * Timer 0 setup.
   */

void st_lld_init(void) {
  OCR0A = 128;
  TIMSK0  |= (1 << OCIE0A);
}

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
#endif  // __AVR__
