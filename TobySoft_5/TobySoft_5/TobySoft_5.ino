 /////////////////////////////////////////////////////////////////////////////////////////////////////////
// For AutoPilot                                                                                       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AUTOPILOT_ON = false;
bool SAFEMODE_ON = true;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Ultrasonic                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ultrasonicSensorQuantity 3 // Number of ultrasonic sensors hooked up
#define ULTRASONIC_INTERVAL 20000  // needs some tuning (30000 perhaps minimum)
unsigned long lastUltrasonicTrigger = 0;
volatile bool ultrasonicInterruptCalled = false;

// unsigned long ultrasonicResponseDurations[ultrasonicSensorQuantity];

int ultrasonicDistances[ultrasonicSensorQuantity]; // array of most recent ultrasonic distance reads
// Historical distance readings
int A13_History[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int A14_History[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int A15_History[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int ultrasonicHistoryPointer = 0; // Pointer for referencing position in history arrays

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For TOF10120                                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int tofReadDistance = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR Proximity                                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int irProxValueFL = 0;
int irProxValueFC = 0;
int irProxValueFR = 0;
// Drop watch
int irDropValueFL = 0;
int irDropValueFR = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Auto-Pilot                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
String DRIVE_INSTRUCTION;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For NRF24L01                                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int dataFromTransmitter[4]; // for receive data, must match transmitter datatype/size!

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For DF Player                                                                                       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MUSIC_ON = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  // Set up Serials
  Serial.begin(115200); // Serial0 - Main debug/USB serial
  Serial2.begin(9600);  // Hardware Serial2 for DFPlayer -- seems to need 9600 baud

  // Initiate robot systems
  initUltrasonicInterrupts(); // Must init Ultrasonic Interrupts before NRF24L01
  initIR_Remote();
  initNRF24();
  initDFPLAYER();
  initOLED();
  initTOF10120();
  initIRProx();
  initAutoPilot();
}

void loop()
{
  // Check for RF instructions
  radioLink();

  // Check for IR remote signal
  checkIR_Remote();

  // Manage HC-SR04 Triggers
  triggerSensors();

  // Manage HC-SR04 interrupts
  if (ultrasonicInterruptCalled)
  {
    resolveUltrasonicInterrupt();
  }

  // Manage TOF10120
  readTOFDistance();

  // Check IR Proximity sensors
  checkIRProx();

  // Auto-Pilot
  if (AUTOPILOT_ON)
  {
    handleNavigation();
  }
  else
  {
    manualControl();
  }
}
