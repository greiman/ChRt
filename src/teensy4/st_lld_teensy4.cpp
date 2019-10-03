#include "Arduino.h"
#if defined(__IMXRT1062__)
#include "../hal/osal.h"
//------------------------------------------------------------------------------
extern "C" void HardFault_Handler(void);
extern "C" void BusFault_Handler(void);
extern "C" void UsageFault_Handler(void);
extern "C" void MemManage_Handler(void);
extern "C" void SVC_Handler(void);
static void setVectors() {
  _VectorsRam[11] = SVC_Handler;
  _VectorsRam[3] = HardFault_Handler;
  _VectorsRam[4] = BusFault_Handler;
  _VectorsRam[5] = UsageFault_Handler;
  _VectorsRam[6] = MemManage_Handler;  
}
//------------------------------------------------------------------------------
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
  setVectors();
  chTimer.begin(tick, 1000000/OSAL_ST_FREQUENCY);
}
#endif  // OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
//------------------------------------------------------------------------------
#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING)
#if 2000000 % OSAL_ST_FREQUENCY != 0
#error "CH_CFG_ST_FREQUENCY is not obtainable due to integer rounding"
#endif
#if OSAL_ST_FREQUENCY > 1000000
#error "CH_CFG_ST_FREQUENCY is too high"
#endif
#if (2000000/OSAL_ST_FREQUENCY) - 1 > 0xFFF
#error "CH_CFG_ST_FREQUENCY is too low due to prescaler limit"
#endif
//------------------------------------------------------------------------------
static void tick() {
  CH_IRQ_PROLOGUE();  
  GPT2_SR |= GPT_SR_OF1;
  __DSB();  // DSB fix for ARM errata 838869
  chSysLockFromISR();   
  chSysTimerHandlerI();
  chSysUnlockFromISR(); 
  CH_IRQ_EPILOGUE();
}
//------------------------------------------------------------------------------
// To ensure proper operations of GPT, the external clock input frequency 
// should be less than 1/4 of frequency of the peripheral clock (ipg_clk).
// This means GPT_PR_PRESCALER24M must be greater than 4.
extern "C" void st_lld_init(void) {
  setVectors();  
  CCM_CCGR0 |= CCM_CCGR0_GPT2_BUS(CCM_CCGR_ON) ;  // enable GPT2 module
  GPT2_CR = 0;
  // Reduce input frequency to 2000000 MHz. 24 Mhz crystal prescale by 12. 
  GPT2_PR = GPT_PR_PRESCALER24M(11) | ((2000000/OSAL_ST_FREQUENCY) - 1);
  GPT2_OCR1 = 0;
  GPT2_SR = 0x3F; // clear all prior status
  GPT2_IR = 0;
  // GPT Enable, Free-Run mode, enable 24MHz crystal, source 24MHz crystal.
  GPT2_CR = GPT_CR_EN | GPT_CR_FRR | GPT_CR_EN_24M | GPT_CR_CLKSRC(5);
  attachInterruptVector(IRQ_GPT2, tick); 
  NVIC_ENABLE_IRQ(IRQ_GPT2);  
}
#endif  // (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING)
#endif  // defined(__IMXRT1062__)