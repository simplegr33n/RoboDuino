/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Ultrasonic                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ultrasonicSensorQuantity 3 // Number of ultrasonic sensors hooked up
#define ULTRASONIC_INTERVAL 20000  // needs some tuning (30000 perhaps minimum)
unsigned long lastUltrasonicTrigger = 0;
volatile bool ultrasonicInterruptCalled = false;
int ultrasonicDistances[ultrasonicSensorQuantity]; // array of most recent ultrasonic distance reads

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For TOF10120                                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int tofReadDistance = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initDistanceSensors()
{
    Serial.println("TobySoft Distance Sensors initiating");

    initUltrasonicInterrupts();
    initIR_Remote();
    initTOF10120();
    initIRProx();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Distance Check functions                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void getDistances()
{
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

    // Get best Front-Left distance
    frontLeftDistance = ultrasonicDistances[1];
    if (irProxValueFL == 0)
    {
        frontLeftDistance = 1;
    }

    // Get best Front-Center distance
    // frontMiddleDistance = ultrasonicDistances[0]; // TODO: FIX ultrasonic mid... ignoring for now
    // if (tofReadDistance < frontMiddleDistance)
    // {
    //     frontMiddleDistance = tofReadDistance;
    // }
    frontMiddleDistance = tofReadDistance;
    if (irProxValueFC == 0)
    {
        frontMiddleDistance = 1;
    }

    // Get best Front-Right distance
    frontRightDistance = ultrasonicDistances[2];
    if (irProxValueFR == 0)
    {
        frontRightDistance = 1;
    }
}
