#ifndef chconf_samd_h
#define chconf_samd_h

/**
 * @brief   System tick frequency.
 * @details Frequency of the system timer that drives the system ticks. This
 *          setting also defines the system tick time unit.
 */
#if !defined(CH_CFG_ST_FREQUENCY)
#define CH_CFG_ST_FREQUENCY                 1000  // 10000 WHG
#endif

/**
 * @brief   Time delta constant for the tick-less mode.
 * @note    If this value is zero then the system uses the classic
 *          periodic tick. This value represents the minimum number
 *          of ticks that is safe to specify in a timeout directive.
 *          The value one is not valid, timeouts are rounded up to
 *          this value.
 */
#if !defined(CH_CFG_ST_TIMEDELTA)
#define CH_CFG_ST_TIMEDELTA                 0 // was 2 WHG
#endif

/**
 * @brief   Round robin interval.
 * @details This constant is the number of system ticks allowed for the
 *          threads before preemption occurs. Setting this value to zero
 *          disables the preemption for threads with equal priority and the
 *          round robin becomes cooperative. Note that higher priority
 *          threads can still preempt, the kernel is always preemptive.
 * @note    Disabling the round robin preemption makes the kernel more compact
 *          and generally faster.
 * @note    The round robin preemption is not supported in tickless mode and
 *          must be set to zero in that case.
 */
#if !defined(CH_CFG_TIME_QUANTUM)
#define CH_CFG_TIME_QUANTUM                 20
#endif

/**
 * @brief   Managed RAM size.
 * @details Size of the RAM area to be managed by the OS. If set to zero
 *          then the whole available RAM is used. The core memory is made
 *          available to the heap allocator and/or can be used directly through
 *          the simplified core memory allocator.
 *
 * @note    In order to let the OS manage the whole RAM the linker script must
 *          provide the @p __heap_base__ and @p __heap_end__ symbols.
 * @note    Requires @p CH_CFG_USE_MEMCORE.
 */
#if !defined(CH_CFG_MEMCORE_SIZE)
#define CH_CFG_MEMCORE_SIZE                 128// was 0 WHG
#endif

/**
 * @brief   Time Measurement APIs.
 * @details If enabled then the time measurement APIs are included in
 *          the kernel.
 *
 * @note    The default is @p TRUE.
 */
#if !defined(CH_CFG_USE_TM)
#define CH_CFG_USE_TM                       FALSE  // was TRUE WHG
#endif

/**
 * @brief   Heap Allocator APIs.
 * @details If enabled then the memory heap allocator APIs are included
 *          in the kernel.
 *
 * @note    The default is @p TRUE.
 * @note    Requires @p CH_CFG_USE_MEMCORE and either @p CH_CFG_USE_MUTEXES or
 *          @p CH_CFG_USE_SEMAPHORES.
 * @note    Mutexes are recommended.
 */
#if !defined(CH_CFG_USE_HEAP)
#define CH_CFG_USE_HEAP                     FALSE  // was TRUE WHG
#endif

/**
 * @brief   Objects Factory APIs.
 * @details If enabled then the objects factory APIs are included in the
 *          kernel.
 *
 * @note    The default is @p FALSE.
 */
#if !defined(CH_CFG_USE_FACTORY)
#define CH_CFG_USE_FACTORY                  FALSE // was TRUE WHG
#endif

/**
 * @brief   Debug option, system state check.
 * @details If enabled the correct call protocol for system APIs is checked
 *          at runtime.
 *
 * @note    The default is @p FALSE.
 */
#if !defined(CH_DBG_SYSTEM_STATE_CHECK)
#define CH_DBG_SYSTEM_STATE_CHECK           FALSE
#endif

/**
 * @brief   Debug option, parameters checks.
 * @details If enabled then the checks on the API functions input
 *          parameters are activated.
 *
 * @note    The default is @p FALSE.
 */
#if !defined(CH_DBG_ENABLE_CHECKS)
#define CH_DBG_ENABLE_CHECKS                FALSE
#endif

/**
 * @brief   Debug option, consistency checks.
 * @details If enabled then all the assertions in the kernel code are
 *          activated. This includes consistency checks inside the kernel,
 *          runtime anomalies and port-defined checks.
 *
 * @note    The default is @p FALSE.
 */
#if !defined(CH_DBG_ENABLE_ASSERTS)
#define CH_DBG_ENABLE_ASSERTS               FALSE
#endif

/**
 * @brief   Debug option, trace buffer.
 * @details If enabled then the trace buffer is activated.
 *
 * @note    The default is @p CH_DBG_TRACE_MASK_DISABLED.
 */
#if !defined(CH_DBG_TRACE_MASK)
#define CH_DBG_TRACE_MASK                   CH_DBG_TRACE_MASK_DISABLED
#endif

/**
 * @brief   Debug option, stack checks.
 * @details If enabled then a runtime stack check is performed.
 *
 * @note    The default is @p FALSE.
 * @note    The stack check is performed in a architecture/port dependent way.
 *          It may not be implemented or some ports.
 * @note    The default failure mode is to halt the system with the global
 *          @p panic_msg variable set to @p NULL.
 */
#if !defined(CH_DBG_ENABLE_STACK_CHECK)
#define CH_DBG_ENABLE_STACK_CHECK           FALSE
#endif

/**
 * @brief   Debug option, stacks initialization.
 * @details If enabled then the threads working area is filled with a byte
 *          value when a thread is created. This can be useful for the
 *          runtime measurement of the used stack.
 *
 * @note    The default is @p FALSE.
 */
#if !defined(CH_DBG_FILL_THREADS)
#define CH_DBG_FILL_THREADS                 TRUE  // was FALSE WHG
#endif
#endif  /* chconf_samd_h */