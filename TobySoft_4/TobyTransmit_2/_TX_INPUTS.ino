#define JOYSTICK0_X_PIN A0
#define JOYSTICK0_Y_PIN A1
#define JOYSTICK0_SW_Pin 2

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initInputs()
{
    // Set up pin for joystick switch function
    pinMode(JOYSTICK0_SW_Pin, INPUT);
    digitalWrite(JOYSTICK0_SW_Pin, HIGH);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input functions                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateInputs()
{
    getJoystick0Values();
}

void getJoystick0Values() // Joystick X value
{
    int xPotValue = analogRead(JOYSTICK0_X_PIN); // Joystick X value
    joystick0ValueX = map(xPotValue, 0, 1023, 0, 180);
    int yPotValue = analogRead(JOYSTICK0_Y_PIN); // Joystick Y value
    joystick0ValueY = map(yPotValue, 0, 1023, 0, 180);
    joystick0ValueSw = digitalRead(JOYSTICK0_SW_Pin); // Joystick Switch value
}
