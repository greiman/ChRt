#ifdef  __AVR__
#include "../hal/osal.h"
#if (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) || defined(__DOXYGEN__)
#if OSAL_ST_FREQUENCY != (1000000UL/1024)
#error "AVR only supports CH_CFG_ST_FREQUENCY == (1000000UL/1024)"
#endif
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
//------------------------------------------------------------------------------
#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) || defined(__DOXYGEN__)
#if OSAL_ST_FREQUENCY != (F_CPU/1024)
#error "AVR only supports CH_CFG_ST_FREQUENCY == (F_CPU/1024)"
#endif
#define PRESCALER (_BV(CS12) | _BV(CS10))
/**
 * @brief Timer handler for free running mode.
 */
OSAL_IRQ_HANDLER(TIMER1_COMPA_vect) {

  OSAL_IRQ_PROLOGUE();

  /* TODO: reset status if required. */

  osalSysLockFromISR();
  osalOsTimerHandlerI();
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}
/**
 * @brief   Low level ST driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {
  /*
   * Periodic mode uses Timer 1 (16 bit).
   */

  /* CTC mode, no clock source. */
  TCCR1A     = 0;
  TCCR1B     = _BV(WGM12);

  /* start disabled. */
  TCCR1C     = 0;
  OCR1A      = 0;
  TCNT1      = 0;
  TIFR_REG   = _BV(OCF1A);                              /* Reset pending.   */    
  TIMSK_REG  = 0;
  TCCR1B     = PRESCALER;
}
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
#endif  // __AVR__
