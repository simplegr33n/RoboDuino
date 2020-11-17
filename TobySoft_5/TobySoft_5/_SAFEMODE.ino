/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safety Check functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORWARD
bool checkFowardSafety()
{
    if ((frontMiddleDistance > 15) && (frontLeftDistance > 10) && (frontRightDistance > 10) && (irDropValueFL == 0) && (irDropValueFR == 0))
    {
        BLOCKED_DRIVE_COUNT = 0;
        return true;
    }

    BLOCKED_DRIVE_COUNT++;
    return false;
}

// LEFT
bool checkLeftSafety()
{
    if ((frontMiddleDistance > 15) && (frontLeftDistance > 10) && (irDropValueFL == 0))
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
    if ((frontMiddleDistance > 15) && (frontRightDistance > 10) && (irDropValueFR == 0))
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
