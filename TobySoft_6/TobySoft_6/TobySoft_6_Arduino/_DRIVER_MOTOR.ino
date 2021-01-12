/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     ///////////////                                                 //
//                                     END GLOBAL VARS                                                 //
//                                     ///////////////                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ======================================== INIT ===================================================== //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void initDriveMotors()
{
    Serial.print("Starting motors...");

    // Set up Motor Output Pins
    pinMode(MOTOR_IN1_PIN, OUTPUT);
    pinMode(MOTOR_IN2_PIN, OUTPUT);
    pinMode(MOTOR_IN3_PIN, OUTPUT);
    pinMode(MOTOR_IN4_PIN, OUTPUT);
    pinMode(MOTOR_ENA_PIN, OUTPUT);
    pinMode(MOTOR_ENB_PIN, OUTPUT);

    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    digitalWrite(MOTOR_IN3_PIN, LOW);
    digitalWrite(MOTOR_IN4_PIN, LOW);
    digitalWrite(MOTOR_ENA_PIN, LOW);
    digitalWrite(MOTOR_ENB_PIN, LOW);

    stopDrive(0); // Stop for good measure..
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motor Driving functions                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void forwardDrive(int speed)
{
    DRIVE_INSTRUCTION = "FORWARD";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkForwardSafety())
        {
            analogWrite(MOTOR_ENA_PIN, speed);
            analogWrite(MOTOR_ENB_PIN, speed);
            digitalWrite(MOTOR_IN1_PIN, LOW);
            digitalWrite(MOTOR_IN2_PIN, HIGH);
            digitalWrite(MOTOR_IN3_PIN, HIGH);
            digitalWrite(MOTOR_IN4_PIN, LOW);
        }
    }
    else // if in Free mode, simply execute instruction
    {
        analogWrite(MOTOR_ENA_PIN, speed);
        analogWrite(MOTOR_ENB_PIN, speed);
        digitalWrite(MOTOR_IN1_PIN, LOW);
        digitalWrite(MOTOR_IN2_PIN, HIGH);
        digitalWrite(MOTOR_IN3_PIN, HIGH);
        digitalWrite(MOTOR_IN4_PIN, LOW);
    }
}

void reverseDrive(int speed)
{
    DRIVE_INSTRUCTION = "REVERSE";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkReverseSafety())
        {
            analogWrite(MOTOR_ENA_PIN, speed);
            analogWrite(MOTOR_ENB_PIN, speed);
            digitalWrite(MOTOR_IN1_PIN, HIGH);
            digitalWrite(MOTOR_IN2_PIN, LOW);
            digitalWrite(MOTOR_IN3_PIN, LOW);
            digitalWrite(MOTOR_IN4_PIN, HIGH);
        }
    }
    else // if in Free mode, simply execute instruction
    {
        analogWrite(MOTOR_ENA_PIN, speed);
        analogWrite(MOTOR_ENB_PIN, speed);
        digitalWrite(MOTOR_IN1_PIN, HIGH);
        digitalWrite(MOTOR_IN2_PIN, LOW);
        digitalWrite(MOTOR_IN3_PIN, LOW);
        digitalWrite(MOTOR_IN4_PIN, HIGH);
    }
}

void leftDrive(int speed)
{
    DRIVE_INSTRUCTION = "LEFT";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkLeftSafety())
        {
            analogWrite(MOTOR_ENA_PIN, speed);
            analogWrite(MOTOR_ENB_PIN, speed);
            digitalWrite(MOTOR_IN1_PIN, HIGH);
            digitalWrite(MOTOR_IN2_PIN, LOW);
            digitalWrite(MOTOR_IN3_PIN, HIGH);
            digitalWrite(MOTOR_IN4_PIN, LOW);
        }
    }
    else // if in Free mode, simply execute instruction
    {
        analogWrite(MOTOR_ENA_PIN, speed);
        analogWrite(MOTOR_ENB_PIN, speed);
        digitalWrite(MOTOR_IN1_PIN, HIGH);
        digitalWrite(MOTOR_IN2_PIN, LOW);
        digitalWrite(MOTOR_IN3_PIN, HIGH);
        digitalWrite(MOTOR_IN4_PIN, LOW);
    }
}

void rightDrive(int speed)
{
    DRIVE_INSTRUCTION = "RIGHT";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkRightSafety())
        {
            analogWrite(MOTOR_ENA_PIN, speed);
            analogWrite(MOTOR_ENB_PIN, speed);
            digitalWrite(MOTOR_IN1_PIN, LOW);
            digitalWrite(MOTOR_IN2_PIN, HIGH);
            digitalWrite(MOTOR_IN3_PIN, LOW);
            digitalWrite(MOTOR_IN4_PIN, HIGH);
        }
    }
    else // if in Free mode, simply execute instruction
    {

        analogWrite(MOTOR_ENA_PIN, speed);
        analogWrite(MOTOR_ENB_PIN, speed);
        digitalWrite(MOTOR_IN1_PIN, LOW);
        digitalWrite(MOTOR_IN2_PIN, HIGH);
        digitalWrite(MOTOR_IN3_PIN, LOW);
        digitalWrite(MOTOR_IN4_PIN, HIGH);
    }
}

void stopDrive(int speed)
{
    DRIVE_INSTRUCTION = "STOP";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        //// TODO: if crossing a cliff, apply small reverse of direction
        if (checkStopSafety())
        {
            digitalWrite(MOTOR_ENA_PIN, LOW);
            digitalWrite(MOTOR_ENB_PIN, LOW);
        }
    }
    else // if in Free mode, simply execute instruction
    {
        digitalWrite(MOTOR_ENA_PIN, LOW);
        digitalWrite(MOTOR_ENB_PIN, LOW);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safe-Mode Check functions                                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORWARD
bool checkForwardSafety()
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
// Safe-Mode  Toggle function                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleSafeMode()
{
    lastSafeModeToggle = micros();
    driveMotors(0, 0); // stop
    SAFEMODE_ON = !SAFEMODE_ON;
    return;
}
