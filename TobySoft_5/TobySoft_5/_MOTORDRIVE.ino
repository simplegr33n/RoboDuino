// L298N Motor Controller pins
#define MOTOR_ENB 5
#define MOTOR_IN1 7
#define MOTOR_IN2 8
#define MOTOR_IN3 9
#define MOTOR_IN4 11
#define MOTOR_ENA 6

#define carSpeed 140

// Advanced function variables
#define fullTurn160 3000000 // millis duration for a 360 degree turn at 160 speed
int adv_turnDegrees = 360;
bool adv_rightDegrees = false;
bool adv_leftDegrees = false;
bool adv_findOpenRight = false;
bool adv_findOpenLeft = false;
bool adv_backRightEvade = false;
bool adv_backLeftEvade = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initDriveMotors()
{
    // Set up Motor Output Pins
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
    pinMode(MOTOR_ENA, OUTPUT);
    pinMode(MOTOR_ENB, OUTPUT);
    stopDrive(0); // Stop for good measure..
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Instruction Filter function                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void driveMotors(int direction)
{
    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        // Advanced Drive functions
        if (adv_rightDegrees)
        {
            rightDegrees(adv_turnDegrees);
            return;
        }
        if (adv_leftDegrees)
        {
            leftDegrees(adv_turnDegrees);
            return;
        }
        if (adv_findOpenRight)
        {
            findClearRight();
            return;
        }
        if (adv_findOpenLeft)
        {
            findClearLeft();
            return;
        }

        if (checkForCollision())
        {
            return; // ignore basic instructions to motors if collision detected
        }

        // Basic Drive functions
        switch (direction)
        {
        case 0: // Stop
            if (checkStopSafety())
            {
                stopDrive(0);
            }
            break;
        case 1: // Forward
            if (checkFowardSafety())
            {
                forwardDrive(carSpeed);
            }
            break;
        case -1: // Reverse
            if (checkReverseSafety())
            {
                reverseDrive(carSpeed);
            }
            break;
        case 2: // Left
            if (checkLeftSafety())
            {
                leftDrive(carSpeed);
            }
            break;
        case 3: // Right
            if (checkRightSafety())
            {
                rightDrive(carSpeed);
            }
            break;
        default:
            break;
        }
    }
    else // Free control mode
    {
        switch (direction)
        {
        case 0: // Stop
            stopDrive(0);
            break;
        case 1: // Forward
            forwardDrive(carSpeed);
            break;
        case -1: // Reverse
            reverseDrive(carSpeed);
            break;
        case 2: // Left
            leftDrive(carSpeed);
            break;
        case 3: // Right
            rightDrive(carSpeed);
            break;
        default:
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motor Driving functions                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void forwardDrive(int speed)
{
    DRIVE_INSTRUCTION = "FORWARD";
    analogWrite(MOTOR_ENA, speed);
    analogWrite(MOTOR_ENB, speed);
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, HIGH);
}

void reverseDrive(int speed)
{
    DRIVE_INSTRUCTION = "REVERSE";
    analogWrite(MOTOR_ENA, speed);
    analogWrite(MOTOR_ENB, speed);
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);
}

void leftDrive(int speed)
{
    DRIVE_INSTRUCTION = "LEFT";
    analogWrite(MOTOR_ENA, speed);
    analogWrite(MOTOR_ENB, speed);
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, HIGH);
}

void rightDrive(int speed)
{
    DRIVE_INSTRUCTION = "RIGHT";
    analogWrite(MOTOR_ENA, speed);
    analogWrite(MOTOR_ENB, speed);
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);
}

void stopDrive(int speed) // TODO: speed argument applies a small reverse/delay() if not 0, in case of sudden stop at dropoff
{
    DRIVE_INSTRUCTION = "STOP";
    digitalWrite(MOTOR_ENA, LOW);
    digitalWrite(MOTOR_ENB, LOW);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Advanced Driving functions                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void leftDegrees(int degrees)
{
    if ((AUTOPILOT_ON || SAFEMODE_ON) && (checkLeftSafety() == false))
    {
        clearAdvancedFunctions();
        playWarningSound();
    }

    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_leftDegrees = true;
    }
    else
    {
        if (micros() - advancedFunctionStart < ((fullTurn160 / 360) * degrees))
        {
            leftDrive(160);
        }
        else
        {
            clearAdvancedFunctions();
        }
    }
}

void rightDegrees(int degrees)
{
    if ((AUTOPILOT_ON || SAFEMODE_ON) && (checkRightSafety() == false))
    {
        clearAdvancedFunctions();
        playWarningSound();
    }

    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_rightDegrees = true;
    }
    else
    {
        if (micros() - advancedFunctionStart < ((fullTurn160 / 360) * degrees))
        {
            rightDrive(160);
        }
        else
        {
            clearAdvancedFunctions();
        }
    }
}

void findClearLeft()
{
    if ((AUTOPILOT_ON || SAFEMODE_ON) && (checkLeftSafety() == false))
    {
        clearAdvancedFunctions();
        playWarningSound();
    }

    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_findOpenLeft = true;
    }
    else
    {
        if (micros() - advancedFunctionStart > fullTurn160) // give up after a full rotation without a clearing
        {
            clearAdvancedFunctions();
            playErrorSound();
        }

        if (frontMiddleDistance < 20 || frontLeftDistance < 10 || frontRightDistance < 10)
        {
            leftDrive(160);
        }
        else
        {
            clearAdvancedFunctions();
        }
    }
}

void findClearRight()
{
    if ((AUTOPILOT_ON || SAFEMODE_ON) && (checkRightSafety() == false))
    {
        clearAdvancedFunctions();
        playWarningSound();
    }

    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_findOpenRight = true;
    }
    else
    {
        if (micros() - advancedFunctionStart > fullTurn160) // give up after a full rotation without a clearing
        {
            clearAdvancedFunctions();
            playErrorSound();
        }

        if (frontMiddleDistance < 20 || frontLeftDistance < 10 || frontRightDistance < 10)
        {
            rightDrive(160);
        }
        else
        {
            clearAdvancedFunctions();
        }
    }
}

void backLeftEvade()
{
    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_backLeftEvade = true;
    }
    else
    {
        if (micros() - advancedFunctionStart < 500000)
        {
            reverseDrive(140);
            return;
        }
        else if ((micros() - advancedFunctionStart > 500000) && (micros() - advancedFunctionStart < (500000 + (fullTurn160 / 3))))
        {
            if ((AUTOPILOT_ON || SAFEMODE_ON) && (checkLeftSafety() == false))
            {
                clearAdvancedFunctions();
                playWarningSound();
            }
            leftDrive(160);
        }
        else
        {
            clearAdvancedFunctions();
        }
    }
}

void backRightEvade()
{
    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_backRightEvade = true;
    }
    else
    {
        if (micros() - advancedFunctionStart < 500000)
        {
            reverseDrive(140);
            return;
        }
        else if ((micros() - advancedFunctionStart > 500000) && (micros() - advancedFunctionStart < (500000 + (fullTurn160 / 3))))
        {
            if ((AUTOPILOT_ON || SAFEMODE_ON) && (checkRightSafety() == false))
            {
                clearAdvancedFunctions();
                playWarningSound();
            }
            rightDrive(160);
        }
        else
        {
            clearAdvancedFunctions();
        }
    }
}

void clearAdvancedFunctions()
{
    stopDrive(0);
    advancedFunctionStart = 0;
    adv_rightDegrees = false;
    adv_leftDegrees = false;
    adv_findOpenRight = false;
    adv_findOpenLeft = false;
    adv_backRightEvade = false;
    adv_backLeftEvade = false;
}
