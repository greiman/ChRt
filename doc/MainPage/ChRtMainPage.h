/**
\mainpage ChibiOS RTOS library
<CENTER>William Greiman</CENTER>
<CENTER>1 Oct 2019</CENTER>

\section Intro Introduction

Giovanni Di Sirio is the author of ChibiOS. Giovanni has over 20
years of experience in the embedded software field.
 
Giovanni has been at STMicroelectronics since 2003, currently Software 
Architect in the  AUTOSAR (AUTomotive Open System ARchitecture) 
MCAL (Microcontroller Abstraction Layer) development team based in the Arzano
(Naples), automotive division.

This package contains a version of the ChbiOS/RT RTOS for AVR Arduinos,
SAMD Arduinos, Arduino Due, and Teensy 3.x.

The port is based on ChibiOS 19.1.3. The ChibiOS/RT kernel is version 6.0.3.

These systems are packaged as the Arduino library ChRt.

The documentation for ChibiOS/RT 6.0.3 is located here:

http://www.chibios.org/dokuwiki/doku.php?id=chibios:documentation:start


The next two sections, What an RTOS is, and What an RTOS is not, 
are from Giovanni's ChibiOS book at the above website.
 
\section RTOSis What an RTOS is

An RTOS is an operating system whose internal processes are guaranteed to be
compliant with (hard or soft) realtime requirements. The fundamental
qualities of an RTOS are:

  -  Predictability. It is the quality of being predictable in the scheduling behavior.
  -  Deterministic. It is the quality of being able to consistently produce the same results under the same conditions.

RTOS are often confused with “fast” operating systems. While efficiency is a
positive attribute of an RTOS, efficiency alone does not qualifies an OS as
RTOS but it could separate a good RTOS from a not so good one.

\section RTOSisNot What an RTOS is not

An RTOS is not a magic wand, your system will not be “realtime” just because you are using an RTOS, what matters is your system design. The RTOS itself is just a toolbox that offers you the required tools for creating a realtime system, you can use the tools correctly or in the wrong way.


\section multithread Threadsafe and reentrant functions

If this is your first exposure to a RTOS you will likely feel some pain.

You may want to start using ChibiOS in cooperative mode.  See the chCoop.ino
example below.  In this mode preemptive context switches are disabled for
threads of the same priority.

The normal Arduino environment is single-threaded so code does not need
to be reentrant or threadsafe.  With a preemptive RTOS, the same resources
may be accessed concurrently by several threads.

Many arduino libraries and functions are not reentrant or threadsafe.

To protect resource integrity, code written for multithreaded programs
must be reentrant and threadsafe.

Reentrance and thread safety are both related to the way that functions
handle resources. Reentrance and thread safety are separate concepts:
a function can be either reentrant, threadsafe, both, or neither.

A reentrant function does not hold static data over successive calls, nor
does it return a pointer to static data. A reentrant function must not call
non-reentrant functions.

A threadsafe function protects shared resources from concurrent access by
locks. Only one thread can be executing at a time.

The dynamic memory functions malloc and free are not threadsafe.  This
means that libraries like String and SD.h are not thread safe since they
use malloc/free.

SdFat does not use malloc but is not threadsafe.  Notice that I put all
access to the SD in the low priority loop thread to avoid problems.

\section rtosprobs Blocking, deadlocks and priority

You must not use Arduino delay() in other than the lowest priority task.
delay() will block all lower priority threads.

Two of the most common design problems for embedded developers are the
deadlock and the priority inversion problem.  You should start with very
simple designs to avoid these subtle problems.

\section errorcodes Crash error codes

I have installed simple exception error handlers for three ISR vectors.
If an execution fault causes one of these exception, I blink the pin 13 LED.
The codes are:

Hard fault - blink one short flash every two seconds

Bus fault - blink two short flashes every two seconds

Usage fault - blink three short flashes every two seconds

\section Examples
There are a number examples.  Please look at and understand these examples.

It is difficult to allocate the correct amount of stack memory for a thread.
Several of the examples use the chUnusedThreadStack() function to check stack
usage. When a thread is created, the thread workspace is filled with a 0x55
pattern.  chUnusedThreadStack() checks how much of the 0x55 pattern has
not been overwritten.

You should start with a generous stack size and adjust it based on the 
unused size. 

\subsection chBlink Two Thread Blink

The chBlink.ino example demonstrates thread definition, semaphores,
and thread sleep.

Thread 1 waits on a semaphore and turns the LED off when signaled
by thread 2.

Thread 2 turns the LED on, sleeps for a period, signals thread 1 to
turn the LED off, and sleeps for another period.

\subsection blink_print Blink Print Example

The blink/print example in each library is chBlinkPrint.ino.

The blink/print examples has three threads. A high priority thread
blinks an LED, a medium priority thread prints a counter
every second, and a low priority thread increments the counter.

The print thread also checks Serial for input.  The print thread will displays
stack usage information for each thread.

An interesting experiment is to observe the non-atomic behavior
of incrementing count in loop().

Comment out noInterrupts() and interrupts() like this:

@code
//  noInterrupts();
  count++;
//  interrupts();
@endcode

You will then see occasional large counts when the print thread tries
to zero count while the loop() thread is incrementing count.

\subsection context_switch Semaphore Context Switch Time

You need an oscilloscope to run this example.  This example is
chContextTime.ino.

To run this example, connect the scope to pin 13. You will see two pulses.
Measure difference in time between first pulse with no context switch and
the second pulse started in ledControl and ended in ledOffTask.

The difference is the time for the semaphore and a context switch.

\subsection coop_schedule Cooperative Scheduling Example

ChibiOS/RT uses cooperative scheduling when CH_TIME_QUANTUM is set to zero.
This disables preemption for threads with equal priority and the
round robin becomes cooperative. Note that higher priority
threads can still preempt, the kernel is always preemptive.

The chCoop.ino example illustrates this feature.

Note that is is not necessary to protect count or maxDelay in this example
since a context switch can not happen while these variables are accessed.

\subsection data_share Data Sharing Using a Mutex

The chDataSharing.ino example illustrates thread safe data sharing
between two threads.

Thread 1 reads a sensor into temp variables.  Thread 1 calls chMtxLock()
to prevent Thread 2 from accessing the shared data, and copies the
temp variables to the shared area, and then unlocks access to the shared area.

Thread 2 runs every second.  Thread 2 locks the shared data, copies
the shared values to temp variables, and unlocks access to the shared area.

Thread 2 then prints the temp values and unused stack stats.
\subsection Event Event Flags

The chEvent.ino example demonstrates use of event flags. 

\subsection fifo_logger FIFO Data Logger
The fast data logger example is chFifoDataLogger.ino. This example
require connection to an SD socket.

Two semaphores are used to implement a FIFO for data records.  This
uncouples the data acquisition task from the SD write task.  SD card
have unpredictable write latencies that can be over 100 milliseconds.

You need a quality SD card to avoid data overrun errors. Overruns could
be avoided by allocating more memory to the buffer queue.

This example logs a counter as dummy data.  You can replace this with
data from an analog pin or your sensor.

Type any character to terminate the example.  Memory usage information
will be printed.

\subsection delay_jitter  Delay Jitter Time

The chJitter.ino example delays for one tick and measures the time difference
in micros between delay calls.

The min and max times are printed by a lower priority task.

\subsection isr_semaphore Task Scheduling from an Interrupt Service Routine

The chIsrSemaphore.ino example demonstrates how a handler task can be
triggered from an ISR by using a binary semaphore.

\subsection mailbox Mailboxes
The chMailbox.ino example demonstrates use of mailboxes.

\subsection mail_pool Mail and Memory Pool
The chMailPool.ino example demonstrates use of a memory pool and
mailboxes with two senders and one receiver.

\subsection mutex_print Mutex Protecting Serial
The chMutex.ino example shows how to protect a shared resource.  In this case
the mutex is used to share Serial between three threads.  The mutex
prevents print calls from the three threads from being scrambled.

\subsection round_robin Round Robin Scheduling
chRoundRobin.ino is a very simple demonstration of two tasks running
in round robin mode.

\subsection counting_Semaphore Counting Semaphore
chSemaphore.ino demonstrates use of a counting semaphore by three tasks.
Execution is restrict execution of at most two tasks in one region of code.
*/