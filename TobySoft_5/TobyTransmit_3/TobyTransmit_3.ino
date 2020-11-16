/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For Inputs                                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int joystick0ValueX;
int joystick0ValueY;
int joystick0ValueSw;

int bigRedButtonValue;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
    Serial.begin(115200);

    initOLED();   // init display
    initInputs(); // init transimitter inputs
    initNRF24();  // init nrf24 transceiver
}

void loop()
{
    updateInputs(); // get updated input states
    sendRadioCom(); // send RF communication via NRF24L01
}
