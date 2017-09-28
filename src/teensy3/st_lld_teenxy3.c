#include "Arduino.h"
#if defined(__arm__) && defined(CORE_TEENSY)
#include "../rt/ch.h"

#define WHG_ST_IRQ_PRIORITY 0XF
#define HANDLER_SYSTICK         11      /**< SYS TCK vector id.             */
#define NVIC_PRIORITY_MASK(prio) ((prio) << (8U - (unsigned)__NVIC_PRIO_BITS))

void nvicSetSystemHandlerPriority(uint32_t handler, uint32_t prio) {
  SCB->SHP[handler] = NVIC_PRIORITY_MASK(prio);
}

static int sysTickEnabled = 0;

extern volatile uint32_t systick_millis_count;
static void systick() {
  systick_millis_count++;
  if (sysTickEnabled) {
    CH_IRQ_PROLOGUE();

    chSysLockFromISR();
    chSysTimerHandlerI();
    chSysUnlockFromISR();

    CH_IRQ_EPILOGUE();
  }
}
void st_lld_init(void) {
  nvicSetSystemHandlerPriority(HANDLER_SYSTICK, WHG_ST_IRQ_PRIORITY);
  _VectorsRam[15] = systick;
  sysTickEnabled = 1;  
}
#endif  // #if defined(__arm__) && defined(CORE_TEENSY);