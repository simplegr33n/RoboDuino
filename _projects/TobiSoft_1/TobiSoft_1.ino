/////////////////////
// For Ultrasonic
/////////////////////
#define ultrasonicSensorQuantity 3 // Number of ultrasonic sensors hooked up
// Interrupt variables
volatile uint8_t portDstate, portDpast, portDchangedBits;
volatile bool ultrasonicInterruptCalled = false;
int ultrasonicResponseCount = 0; // trigger sensors when ultrasonicResponseCount == ultrasonicSensorQuantity
unsigned long ultrasonicResponseDurations[ultrasonicSensorQuantity];
// Historical distance readings
int D2_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D3_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D4_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int ultrasonicHistoryPointer = 0; // Pointer for referencing position in history arrays

///////////////////////
// For TOF10120
///////////////////////
int tofReadDistance = 0;
unsigned long lastTOFRead = 0; // micros() timestamp of last TOF10120 read

///////////////////////
// For Auto Pilot
///////////////////////
unsigned long lastPilotDecision = 0; // micros() timestamp of last autoPilotDecision

///////////////////////
//                   //
//  END GLOBAL VARS  //
//                   //
///////////////////////

void setup()
{
  Serial.begin(9600);

  initOLED();

  initUltrasonicInterrupts();

  initTOF10120();

  initDFPLAYER();

  initAutoPilot();
}

void loop()
{
  // Manage ultrasonic interrupts
  if (ultrasonicInterruptCalled)
  {
    resolveUltrasonicInterrupt();
  }

  // Manage TOF10120
  if ((lastTOFRead == 0) || (micros() - lastTOFRead > 20000))
  {
    lastTOFRead = micros();
    readTOFDistance();
  }

  // Update OLED Display
  if (ultrasonicResponseCount == ultrasonicSensorQuantity)
  {
    updateUltrasonicHistory();
    updateUltrasonicGraph();
  }

  // Auto-Pilot
  if ((lastPilotDecision == 0) || (micros() - lastPilotDecision > 40000))
  {
    lastPilotDecision = micros();
    autoNavigation();
  }
}
