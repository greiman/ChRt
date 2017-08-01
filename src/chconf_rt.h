#ifndef chconf_rt_h
#define chconf_rt_h
#define _CHIBIOS_RT_CONF_
#ifdef __AVR__
#include "avr/chconf_avr.h"
#elif defined(__arm__)
#include "arm/chconf_arm.h"
#else 
#error "unknown processor type"
#endif  // __AVR__
#ifdef __cplusplus
extern "C" {
#endif
void dbgPrint(int n);
void errorBlink(int n);
#ifdef __cplusplus
}
#endif

#endif  // chconf_rt_h
