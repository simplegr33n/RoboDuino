/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For main loop()                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AUTOPILOT_ON = false;
bool MUSIC_ON = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Ultrasonic                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ultrasonicSensorQuantity 3 // Number of ultrasonic sensors hooked up
// Interrupt variables
volatile uint8_t portKstate, portKpast, changedBitsK;
volatile bool ultrasonicInterruptCalled = false;
int ultrasonicResponseCount = 0; // trigger sensors when ultrasonicResponseCount == ultrasonicSensorQuantity
unsigned long ultrasonicResponseDurations[ultrasonicSensorQuantity];
// Historical distance readings
int D2_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D3_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D4_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int ultrasonicHistoryPointer = 0; // Pointer for referencing position in history arrays

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For TOF10120                                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int tofReadDistance = 0;
unsigned long lastTOFRead = 0; // micros() timestamp of last TOF10120 read

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Auto-Pilot                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long lastPilotDecision = 0; // micros() timestamp of last autoPilotDecision

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200); // Serial0 - Main debug/USB serial

  Serial2.begin(9600); // Hardware Serial2 for DFPlayer -- seems to need 9600 baud

  initIR_Remote();

  initDFPLAYER();

  initOLED();

  initUltrasonicInterrupts();

  initTOF10120();

  initAutoPilot();
}

void loop()
{
  // Check for IR remote signal
  checkIR_Remote();

  // Manage HC-SR04 interrupts
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
    if (AUTOPILOT_ON) // TODO: remove... for now just so i can enjoy sleep screen, eventually need more advanced display system.
    {
      updateUltrasonicGraph();
    }
  }

  // Auto-Pilot
  if (AUTOPILOT_ON)
  {
    if ((lastPilotDecision == 0) || (micros() - lastPilotDecision > 40000))
    {
      lastPilotDecision = micros();
      autoNavigation();
    }
  }
  else
  {
    displaySleepScreen();
  }
}
