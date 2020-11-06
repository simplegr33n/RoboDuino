#include <stdint.h>
#include <stdbool.h>

/////////////////////
// For Ultrasonic
/////////////////////
#define ultrasonicSensorQuantity 3 // Number of ultrasonic sensors hooked up
// Interrupt variables
volatile uint8_t portDstate, portDpast, changedBits;
volatile bool ultrasonicInterruptCalled = false;
int ultrasonicResponseCount = 0; // trigger sensors when ultrasonicResponseCount == ultrasonicSensorQuantity
unsigned long ultrasonicResponseDurations[ultrasonicSensorQuantity];
// Historical distance readings
int D2_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D3_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D4_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int ultrasonicHistoryPointer = 0; // Pointer for referencing position in history arrays

///////////////////////
// For Auto Pilot
///////////////////////
unsigned long lastPilotDecision = 0; // micros() timestamp of last autoPilotDecision

void setup()
{
  Serial.begin(9600);

  initOLED();

  initUltrasonicInterrupts();

  initAutoPilot();
}

void loop()
{

  if (ultrasonicInterruptCalled)
  {
    resolveUltrasonicInterrupt();
  }

  if (ultrasonicResponseCount == ultrasonicSensorQuantity)
  {
    updateUltrasonicHistory();

    drawGraph();
  }

  // Drive car code
  if ((lastPilotDecision == 0) || (micros() - lastPilotDecision > 100000))
  {
    lastPilotDecision = micros();
    autoNavigation();
  }
}
