/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safety Check functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORWARD
bool checkFowardSafety()
{
    // Get best Front-Left distance
    int leftDistance = ultrasonicDistances[1];
    if (irProxValueFL == 0)
    {
        leftDistance = 1;
    }

    // Get best Front-Center distance
    int middleDistance = ultrasonicDistances[0];
    if (tofReadDistance < middleDistance)
    {
        middleDistance = tofReadDistance;
    }
    if (irProxValueFC == 0)
    {
        middleDistance = 1;
    }

    // Get best Front-Right distance
    int rightDistance = ultrasonicDistances[2];
    if (irProxValueFR == 0)
    {
        rightDistance = 1;
    }

    if ((middleDistance > 15) && (leftDistance > 10) && (rightDistance > 10) && (irDropValueFL == 0) && (irDropValueFR == 0))
    {
        BLOCKED_DRIVE_COUNT = 0;
        return true;
    }

    BLOCKED_DRIVE_COUNT++;
    return false;
}

// STOP
bool checkStopSafety()
{
    // Serial.println("No stop safety check yet.");

    BLOCKED_DRIVE_COUNT = 0;
    return true;
}

// REVERSE
bool checkReverseSafety()
{
    Serial.println("No sensors for reverse safety check!");

    BLOCKED_DRIVE_COUNT = 0;
    return true;
}

// LEFT
bool checkLeftSafety()
{
    // Get best Front-Left distance
    int leftDistance = ultrasonicDistances[1];
    if (irProxValueFL == 0)
    {
        leftDistance = 1;
    }

    // Get best Front-Center distance
    int middleDistance = ultrasonicDistances[0];
    if (tofReadDistance < middleDistance)
    {
        middleDistance = tofReadDistance;
    }
    if (irProxValueFC == 0)
    {
        middleDistance = 1;
    }

    if ((middleDistance > 15) && (leftDistance > 10) && (irDropValueFL == 0))
    {
        BLOCKED_DRIVE_COUNT = 0;
        return true;
    }

    BLOCKED_DRIVE_COUNT++;
    return false;
}

// RIGHT
bool checkRightSafety()
{
    // Get best Front-Center distance
    int middleDistance = ultrasonicDistances[0];
    if (tofReadDistance < middleDistance)
    {
        middleDistance = tofReadDistance;
    }
    if (irProxValueFC == 0)
    {
        middleDistance = 1;
    }

    // Get best Front-Right distance
    int rightDistance = ultrasonicDistances[2];
    if (irProxValueFR == 0)
    {
        rightDistance = 1;
    }

    if ((middleDistance > 15) && (rightDistance > 10) && (irDropValueFR == 0))
    {
        BLOCKED_DRIVE_COUNT = 0;
        return true;
    }

    BLOCKED_DRIVE_COUNT++;
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toggle function                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleSafeMode()
{
    playSafeToggleSound();

    lastSafeModeToggle = micros();
    driveMotors(0); // stop
    SAFEMODE_ON = !SAFEMODE_ON;
    return;
}
