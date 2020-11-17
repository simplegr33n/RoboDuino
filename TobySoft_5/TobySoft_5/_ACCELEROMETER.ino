#define accelerometerPinX A1 // x-axis of the accelerometer
float collisionThreshold = 1.35;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool checkForCollision()
{

    if ((DRIVE_INSTRUCTION == "STOP") || (DRIVE_INSTRUCTION == "REVERSE"))
    {
        return false; // Ignore accelerometer when stopped/reversing.
    }

    int x = analogRead(accelerometerPinX); // read from accelerometerPinX

    if (((((float)x - 331.5) / 65) > collisionThreshold)) // collision detected!
    {
        playErrorSound();

        BLOCKED_DRIVE_COUNT++;
        return true; // indicate collision detected
    }
    return false;

    // DEBUG
    // Serial.print(((float)x - 331.5) / 65); //print x value on serial monitor
    // Serial.println("m/s²");
}
