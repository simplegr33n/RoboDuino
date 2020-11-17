/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safety Check functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORWARD
bool checkFowardSafety()
{
    if ((frontMiddleDistance > 15) && (frontLeftDistance > 10) && (frontRightDistance > 10) && (irDropValueFL == 0) && (irDropValueFR == 0))
    {
        evadeTryCount = 0;
        evadeLoopCount = 0;
        BLOCKED_DRIVE_COUNT = 0;
        return true;
    }

    BLOCKED_DRIVE_COUNT++;
    return false;
}

// LEFT
bool checkLeftSafety()
{
    if ((frontLeftDistance > 15) && (irDropValueFL == 0))
    {
        return true;
    }

    BLOCKED_DRIVE_COUNT++;
    return false;
}

// RIGHT
bool checkRightSafety()
{
    if ((frontRightDistance > 15) && (irDropValueFR == 0))
    {
        return true;
    }

    BLOCKED_DRIVE_COUNT++;
    return false;
}

// STOP
bool checkStopSafety()
{
    // Serial.println("No stop safety check yet.");
    return true;
}

// REVERSE
bool checkReverseSafety()
{
    Serial.println("No sensors for reverse safety check!");
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
