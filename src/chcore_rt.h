#ifndef chcore_rt_h
#define chcore_rt_h
#ifdef __AVR__
#include "avr/chcore_avr.h"
#else  // __AVR__ 
#include "arm/chcore_arm.h" 
#endif  // __AVR__
#endif  // chcore_rt_h
