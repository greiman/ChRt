#ifndef chparams_teensy4_h
#define chparams_teensy4_h

/**
 * @brief   Cortex core model.
 */
#define CORTEX_MODEL            7

/**
 * @brief   Floating Point unit presence.
 */
#if defined(__IMXRT1062__)
#define CORTEX_HAS_FPU          1
#else  // CORTEX_HAS_FPU
#error unknown Teensy 4 model
#endif  // CORTEX_HAS_FPU

#define __FPU_PRESENT           CORTEX_HAS_FPU

/**
 * @brief   Number of bits in priority masks.
 */
#define CORTEX_PRIORITY_BITS    4
#define __NVIC_PRIO_BITS        4
#if !defined(_FROM_ASM_)
//#define SVC_Handler svcall_isr
#define PendSV_Handler pendablesrvreq_isr
#undef __disable_irq
#undef __enable_irq
typedef enum IRQn {
  SVCall_IRQn                   =  -5,      /* 11 SV Call Interrupt */
  DebugMonitor_IRQn             =  -4,      /* 12 Debug Monitor Interrupt */
  PendSV_IRQn                   =  -2,      /* 14 Pend SV Interrupt */
  SysTick_IRQn                  =  -1,      /* 15 System Tick Interrupt */
} IRQn_Type;

#include "../CMSIS/core_cm7.h"
#endif  // !defined(_FROM_ASM_)
#endif // chparams_teensy4_h

