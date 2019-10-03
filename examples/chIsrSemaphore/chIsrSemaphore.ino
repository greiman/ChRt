// Example of how a handler task can be triggered from an ISR
// by using a binary semaphore.
#include <ChRt.h>

// Pins to generate interrupts - these pins must be connected with a wire.
const uint8_t INPUT_PIN = 2;
const uint8_t OUTPUT_PIN = 3;

// Initialize semaphore as taken. 
BSEMAPHORE_DECL(isrSem, true);

// ISR entry time
volatile uint32_t tIsr = 0;
//------------------------------------------------------------------------------
// Fake ISR, normally
// void isrFcn() {
// would be replaced by something like
// CH_IRQ_HANDLER(INT0_vect) {
//
void isrFcn() {

  // On ARM CH_IRQ_PROLOGUE is void.
  CH_IRQ_PROLOGUE();
  // IRQ handling code, preemptable if the architecture supports it.

  // Only ISR processing is to save time.
  tIsr = micros();

  chSysLockFromISR();
  // Invocation of some I-Class system APIs, never preemptable.

  // Signal handler task.
  chBSemSignalI(&isrSem);
  chSysUnlockFromISR();

  // More IRQ handling code, again preemptable.

  // Perform rescheduling if required.
  CH_IRQ_EPILOGUE();
}
//------------------------------------------------------------------------------
// Handler task for interrupt.
static THD_WORKING_AREA(waThd1, 200);

static THD_FUNCTION(handler, arg) {
  (void)arg;
  while (true) {
    // wait for event
    chBSemWait(&isrSem);
    
    // print elapsed time
    uint32_t t = micros();
    Serial.print("Handler: ");
    Serial.println(t - tIsr);
  }
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial.
  while (!Serial) {}

  chBegin(mainThread);
  // chBegin() resets stacks and should never return.
  while (true) {}
}
//------------------------------------------------------------------------------
void mainThread() {
  // Setup and check pins.
  pinMode(INPUT_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_PIN, OUTPUT);
  
  for (int i = 0; i < 10; i++) {
    bool level = i & 1;
    digitalWrite(OUTPUT_PIN, level);
    if (level != digitalRead(INPUT_PIN)) {
      Serial.print("pin ");
      Serial.print(INPUT_PIN);
      Serial.print(" must be connected to pin ");
      Serial.println(OUTPUT_PIN);
      while (true) {}
    }
  }   
  // Start handler task.
  chThdCreateStatic(waThd1, sizeof(waThd1), NORMALPRIO + 1, handler, NULL);
 
  // Attach interrupt function.
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), isrFcn, RISING);
  while (true) {
    // Cause an interrupt - normally done by external event.
    Serial.println("High");
    digitalWrite(OUTPUT_PIN, HIGH);
    Serial.println("Low");
    digitalWrite(OUTPUT_PIN, LOW);
    Serial.println();
    chThdSleepMilliseconds(1000);
  }
}
//------------------------------------------------------------------------------
void loop() {
  // Not used.
}