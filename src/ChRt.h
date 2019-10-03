#ifndef ChRt_h
#define ChRt_h
/**
 * \file
 * \brief main ChRt include file.
 */
#include "rt/include/ch.h"
#include "hal_st.h"

/** Size of handler stack. */
#define HANDLER_STACK_SIZE 400
/**
 * Starts ChibiOS/RT - does not return.
 * \param[in] mainThread Function to be called before repeated calls
 *                       to loop().
 */
void chBegin(void (*mainThread)());
/**
 * Determine unused bytes in the handler stack.
 *
 * \return number of unused bytes
 */
size_t chUnusedHandlerStack();
/**
 * Determine unused bytes in the main() thread stack.
 *
 * \return number of unused bytes
 */
size_t chUnusedMainStack();
/**
 * Determine number unused bytes in a thread's stack.
 *
 * \param[in] wsp pointer to working space for thread
 * \param[in] size working space size
 *
 * \return Unused byte count.
 * Most accurate if called from the main thread. Some versions of sbrk()
 * don't work correctly when called from another thread.
 */
size_t chUnusedThreadStack(void *wsp, size_t size);

/** \return Heap end address. Best if called from the main thread. */
uint8_t* heapEnd();
#ifdef __cplusplus
extern "C" {
#endif
  /** Blink an error pattern
   *
   * \param[in] n number of short pulses
   */
  void errorBlink(int n);
#ifdef __cplusplus
}
#endif
#endif  // ChRt_h