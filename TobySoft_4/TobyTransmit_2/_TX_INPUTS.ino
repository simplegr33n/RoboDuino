#define JOYSTICK0_X_Pin A0
#define JOYSTICK0_Y_Pin A1
#define JOYSTICK0_SW_Pin 2

#define BIG_RED_BUTTON_Pin 3

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

    // Set up big red button pin
    pinMode(BIG_RED_BUTTON_Pin, INPUT);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input functions                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateInputs()
{
    getJoystick0Values();
    getButtonValues();
}

void getJoystick0Values() // Joystick X value
{
    int xPotValue = analogRead(JOYSTICK0_X_Pin); // Joystick X value
    joystick0ValueX = map(xPotValue, 0, 1023, 0, 180);
    int yPotValue = analogRead(JOYSTICK0_Y_Pin); // Joystick Y value
    joystick0ValueY = map(yPotValue, 0, 1023, 0, 180);
    joystick0ValueSw = digitalRead(JOYSTICK0_SW_Pin); // Joystick Switch value
}

void getButtonValues() {
    bigRedButtonValue = digitalRead(BIG_RED_BUTTON_Pin); // Big red button value  
}
