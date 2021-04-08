#ifndef chconf_avr_h
#define chconf_avr_h

// must be one, tickless AVR not implemented.
#define USE_AVR_PERIODIC_ST                 1  // WHG

#if USE_AVR_PERIODIC_ST

#define CH_CFG_TIME_QUANTUM                 20  // WHG
/**
 * @brief   System time counter resolution.
 * @note    Allowed values are 16 or 32 bits.
 */
#if !defined(CH_CFG_ST_RESOLUTION)
#define CH_CFG_ST_RESOLUTION                32 // 16 WHG
#endif

/**
 * @brief   System tick frequency.
 * @details Frequency of the system timer that drives the system ticks. This
 *          setting also defines the system tick time unit.
 */
#if !defined(CH_CFG_ST_FREQUENCY)
#define CH_CFG_ST_FREQUENCY                 (1000000UL/1024) // 15624 WHG
#endif

/**
 * @brief   Time intervals data size.
 * @note    Allowed values are 16, 32 or 64 bits.
 */
#if !defined(CH_CFG_INTERVALS_SIZE)
#define CH_CFG_INTERVALS_SIZE               32 // 16 WHG
#endif

/**
 * @brief   Time types data size.
 * @note    Allowed values are 16 or 32 bits.
 */
#if !defined(CH_CFG_TIME_TYPES_SIZE)
#define CH_CFG_TIME_TYPES_SIZE              32 // 16 WHG
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
//------------------------------------------------------------------------------
#else  // USE_AVR_PERIODIC_ST
/**
 * @brief   System time counter resolution.
 * @note    Allowed values are 16 or 32 bits.
 */
#if !defined(CH_CFG_ST_RESOLUTION)
#define CH_CFG_ST_RESOLUTION                16
#endif

/**
 * @brief   System tick frequency.
 * @details Frequency of the system timer that drives the system ticks. This
 *          setting also defines the system tick time unit.
 */
#if !defined(CH_CFG_ST_FREQUENCY)
#define CH_CFG_ST_FREQUENCY                 (F_CPU/1024) // 15624 WHG
#endif

/**
 * @brief   Time intervals data size.
 * @note    Allowed values are 16, 32 or 64 bits.
 */
#if !defined(CH_CFG_INTERVALS_SIZE)
#define CH_CFG_INTERVALS_SIZE               16
#endif

/**
 * @brief   Time types data size.
 * @note    Allowed values are 16 or 32 bits.
 */
#if !defined(CH_CFG_TIME_TYPES_SIZE)
#define CH_CFG_TIME_TYPES_SIZE              16
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
#define CH_CFG_ST_TIMEDELTA                 2
#endif

#endif  // USE_AVR_PERIODIC_ST

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
#define CH_CFG_MEMCORE_SIZE                 4  // 128 WHG
#endif

/**
 * @brief   Time Measurement APIs.
 * @details If enabled then the time measurement APIs are included in
 *          the kernel.
 *
 * @note    The default is @p TRUE.
 */
#if !defined(CH_CFG_USE_TM)
#define CH_CFG_USE_TM                       FALSE
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
#define CH_CFG_USE_HEAP                     FALSE  // TRUE WHG
#endif

/**
 * @brief   Dynamic Threads APIs.
 * @details If enabled then the dynamic threads creation APIs are included                  
 *          in the kernel.
 *
 * @note    The default is @p TRUE.
 * @note    Requires @p CH_CFG_USE_WAITEXIT.
 * @note    Requires @p CH_CFG_USE_HEAP and/or @p CH_CFG_USE_MEMPOOLS.
 */
#if !defined(CH_CFG_USE_DYNAMIC)
#define CH_CFG_USE_DYNAMIC                  FALSE
#endif

/**
 * @brief   Objects Factory APIs.
 * @details If enabled then the objects factory APIs are included in the
 *          kernel.
 *
 * @note    The default is @p FALSE.
 */
#if !defined(CH_CFG_USE_FACTORY)
#define CH_CFG_USE_FACTORY                  FALSE
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
#define chDbgCheck(c)  // WHG prevent warnings
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


#endif  /* chconf_avr_h */
