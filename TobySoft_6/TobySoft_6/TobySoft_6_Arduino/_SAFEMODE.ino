/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safety Check functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORWARD
bool checkFowardSafety()
{
    if ((frontCenterDistance > 15) && (frontLeftDistance > 10) && (frontRightDistance > 10))
    {
        return true;
    }
    return false;
}

// LEFT
bool checkLeftSafety()
{
    if ((frontLeftDistance > 10))
    {
        return true;
    }
    return false;
}

// RIGHT
bool checkRightSafety()
{
    if ((frontRightDistance > 10))
    {
        return true;
    }
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
    if ((backDistance > 10))
    {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toggle function                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleSafeMode()
{
    lastSafeModeToggle = micros();
    driveMotors(0, 0); // stop
    SAFEMODE_ON = !SAFEMODE_ON;
    return;
}
