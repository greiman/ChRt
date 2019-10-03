#include "rt/include/ch.h"
#if defined(__IMXRT1062__)
#include "teensy4/st_lld_teensy4.h"
#elif defined(__AVR__)
#include "avr/st_lld_avr.h"
#else  // defined(__IMXRT1062__)
#ifdef __cplusplus
extern "C" {
#endif
  void st_lld_init(void);
#ifdef __cplusplus
}
#endif
#endif  // defined(__IMXRT1062__)