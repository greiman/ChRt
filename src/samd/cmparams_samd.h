#ifndef cmparams_samd_h
#define cmparams_samd_h

/**
 * @brief   Cortex core model.
 */
#define CORTEX_MODEL            0

/**
 * @brief   Floating Point unit presence.
 */
#define CORTEX_HAS_FPU          0

/**
 * @brief   Number of bits in priority masks.
 */
#define CORTEX_PRIORITY_BITS    2

#if !defined(_FROM_ASM_)
#include "Arduino.h"
#endif  // _FROM_ASM_

#endif /* cmparams_samd_h */

/** @} */
