// Data logger based on a FIFO to decouple SD write latency from data
// acquisition timing.
//
// The FIFO uses two semaphores to synchronize between tasks.
#include "SD.h"
#include "ChRt.h"
//
// Interval between points in units of 1024 usec on AVR, usec on ARM.
#if defined(__AVR__) && OSAL_ST_FREQUENCY < 1000
const systime_t intervalTicks = 4;
#else
const systime_t intervalTicks = TIME_US2I(4000);
#endif
//------------------------------------------------------------------------------
// SD file definitions.
const uint8_t sdChipSelect = SS;
File file;
//------------------------------------------------------------------------------
// Fifo definitions.

// Size of fifo in records.
const size_t FIFO_SIZE = 32;

// Count of data records in fifo.
SEMAPHORE_DECL(fifoData, 0);

// Count of free buffers in fifo.
SEMAPHORE_DECL(fifoSpace, FIFO_SIZE);

// Data type for fifo item.
struct FifoItem_t {
  uint32_t usec;  
  int value;
  int error;
};
// Array of fifo items.
FifoItem_t fifoArray[FIFO_SIZE];
//------------------------------------------------------------------------------
// Declare a stack with 32 bytes beyond task switch and interrupt needs.
static THD_WORKING_AREA(waThread1, 32);

static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  // Index of record to be filled.
  size_t fifoHead = 0;

  // Count of overrun errors.
  int error = 0;

  // Dummy data.
  int count = 0;
  
  systime_t logTimeTicks = chVTGetSystemTime();
  while (true) {
    logTimeTicks += intervalTicks;
    chThdSleepUntil(logTimeTicks);
    // Get a buffer.
    if (chSemWaitTimeout(&fifoSpace, TIME_IMMEDIATE) != MSG_OK) {
      // Fifo full, indicate missed point.
      error++;
      continue;
    }
    FifoItem_t* p = &fifoArray[fifoHead];
    p->usec = micros();

    // Replace next line with data read from sensor such as.
    // p->value = analogRead(0);
    p->value = count++;

    p->error = error;
    error = 0;

    // Signal new data.
    chSemSignal(&fifoData);
    
    // Advance FIFO index.
    fifoHead = fifoHead < (FIFO_SIZE - 1) ? fifoHead + 1 : 0;
  }
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial.
  while (!Serial) {}
  
  // Start kernel -  loop() becomes main thread.
  chBegin(0);
  // chBegin() resets stacks and should never return.
  while (true) {}  
}
//------------------------------------------------------------------------------
// FIFO index for record to be written.
size_t fifoTail = 0;

// Time in micros of last point.
uint32_t last = 0;

// Remember errors.
bool overrunError = false;
void loop() {
  Serial.println(F("type any character to begin"));
  while(!Serial.available()); 
  
  // Open file.
  if (!SD.begin(sdChipSelect)) {
    Serial.println(F("SD begin failed."));
    while (true) {}
  }
  file = SD.open("DATA.CSV", O_CREAT | O_WRITE | O_TRUNC);
  if (!file) {
    Serial.println(F("file open failed."));
    while (true) {} 
  }
  // Throw away input.
  while (Serial.read() >= 0);
  Serial.println(F("type any character to end"));
    
  // Start producer thread.
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);    
  
  // SD write loop.
  while (!Serial.available()) {
    // Wait for next data point.
    chSemWait(&fifoData);

    FifoItem_t* p = &fifoArray[fifoTail];
    if (fifoTail >= FIFO_SIZE) fifoTail = 0;

    // Print interval between points.
    if (last) {
      file.print(p->usec - last);
    } else {
      file.write("NA");
    }
    last = p->usec;
    file.write(',');
    file.print(p->value);
    file.write(',');
    file.println(p->error);

    // Remember error.
    if (p->error) overrunError = true;

    // Release record.
    chSemSignal(&fifoSpace);
    
    // Advance FIFO index.
    fifoTail = fifoTail < (FIFO_SIZE - 1) ? fifoTail + 1 : 0;
  }
  // Close file, print stats and stop.
  file.close();
  Serial.println(F("Done"));
  Serial.print(F("Unused Thread1 stack: "));
  Serial.println(chUnusedThreadStack(waThread1, sizeof(waThread1)));
  Serial.print(F("Unused main stack: "));
  Serial.println(chUnusedMainStack());
  if (overrunError) {
    Serial.println();
    Serial.println(F("** overrun errors **"));
    Serial.println(F("Increase intervalTicks and/or FIFO_SIZE"));
  }
  while (true) {}
}