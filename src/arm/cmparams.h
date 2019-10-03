#ifndef chparams_h
#define chparams_h
#if defined(__IMXRT1062__)
#include "../teensy4/cmparams_teensy4.h"
#elif defined(__MK20DX256__) || defined(__MK20DX128__) || \
    defined(__MK64FX512__) || defined(__MK66FX1M0__)
#include "../teensy3/cmparams_teensy3.h"
#elif defined(__SAMD21G18A__) || defined(__SAMD21J18A__)
#include "../samd/cmparams_samd.h"
#elif defined(__SAM3X8E__) || defined(__SAM3X8H__)
#include "../sam3x/cmparams_sam3x.h"
#else
#error unknown ARM processor
#endif

#endif  // chparams_h