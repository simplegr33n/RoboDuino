/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Main loop()                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SYS_INITIALIZED = false;
bool AUTOPILOT_ON = true;
bool SAFEMODE_ON = false;
//
#define toggleDebounce 500 // half second debounce for rf button toggles
unsigned long lastAutoPilotToggle, lastSafeModeToggle;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Distance Harmonizer                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int frontLeftDistance, frontCenterDistance, frontRightDistance, backDistance;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For IR Proximity                                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int irProxValueFL, irProxValueFCL, irProxValueFCR, irProxValueFR, irProxValueBCL, irProxValueBCR;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Auto-Pilot                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
String DRIVE_INSTRUCTION;
int autoPilotSpeed = 30; // 0-255, constant speed in autoPilot for now

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For NRF24L01                                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int dataFromTransmitter[4] = {-1, -1, -1, -1}; // for receive data, must match transmitter datatype/size!

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Motor Driver                                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long advancedFunctionStart;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For headlights                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <FastLED.h>
#define HEADLIGHTS_NUM 1 // per side
#define DATA_PIN 2
// Define the array of leds
CRGB leds[HEADLIGHTS_NUM];

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  // Set up Serials
  Serial.begin(9600); // Serial0 - Main debug/USB serial

  Serial.println("TobySoft intializing...");

  // Initiate robot systems
  initDistanceSensors(); // Must init before NRF24L01
  initNRF24();
  initDriveMotors();

  //init headlights:
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, HEADLIGHTS_NUM);
  leds[0] = CRGB::White;
  FastLED.show();

  SYS_INITIALIZED = true;
}

void loop()
{
  // get distances through Distance Harmonizer
  getDistances();

  // check NRF24 for radio instructions
  radioLink();

  // control loop
  if (AUTOPILOT_ON)
  {
    autoControl();
  }
  else
  {
    manualControl();
  }
}