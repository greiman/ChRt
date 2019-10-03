#include "Arduino.h"
#include "ChRt.h"

//------------------------------------------------------------------------------
/** calibration factor for delayMS */
#if defined(__IMXRT1062__)
#define CAL_FACTOR (F_CPU/3500)
#else  // defined(__IMXRT1062__)
#define CAL_FACTOR (F_CPU/7000)
#endif  // defined(__IMXRT1062__)
/** delay between led error flashes
 * \param[in] millis milliseconds to delay
 */
static void delayMS(uint32_t millis) {
  uint32_t i;
  uint32_t iterations = millis*CAL_FACTOR;
  for(i = 0; i < iterations; ++i) {
    asm volatile("nop\n\t");
  }
}
//------------------------------------------------------------------------------
void errorBlink(int n) {
	noInterrupts();
  pinMode(13, OUTPUT);
  for (;;) {
    int i;
    for (i = 0; i < n; i++) {
      digitalWrite(13, 1);
      delayMS(300);
      digitalWrite(13, 0);
      delayMS(300);
    }
    delayMS(2000);
  }
}
// catch Teensy and Due exceptions
/** Hard fault - blink one short flash every two seconds */
void hard_fault_isr()	{errorBlink(1);}

/** Hard fault - blink one short flash every two seconds */
void HardFault_Handler() 	{errorBlink(1);}

/** Bus fault - blink two short flashes every two seconds */
void bus_fault_isr() {errorBlink(2);}

/** Bus fault - blink two short flashes every two seconds */
void BusFault_Handler() {errorBlink(2);}

/** Usage fault - blink three short flashes every two seconds */
void usage_fault_isr() {errorBlink(3);}

/** Usage fault - blink three short flashes every two seconds */
void UsageFault_Handler() {errorBlink(3);}

/** Memory management fault - blink four short flashes every two seconds */
void memmanage_fault_isr() {errorBlink(4);}

/** Memory management fault - blink four short flashes every two seconds */
void MemManage_Handler() {errorBlink(4);}