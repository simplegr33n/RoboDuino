// L298N Motor Controller pins
#define MOTOR_ENB 5
#define MOTOR_IN1 7
#define MOTOR_IN2 8
#define MOTOR_IN3 9
#define MOTOR_IN4 11
#define MOTOR_ENA 6

#define carSpeed 140

// Advanced function variables
unsigned long advancedFunctionStart = 0;
bool adv_right360 = false;
bool adv_left360 = false;
bool adv_searchRight = false;
bool adv_searchLeft = false;

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
        if (checkForCollision())
        {
            return; // ignore instructions to motors if collision detected
        }

        // Advanced Drive functions
        if (adv_right360)
        {
            right360();
            return;
        }
        if (adv_left360)
        {
            left360();
            return;
        }
        if (adv_searchRight)
        {
            searchRight();
            return;
        }
        if (adv_searchLeft)
        {
            searchLeft();
            return;
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
void searchLeft()
{
    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
    }
    else
    {
        if (micros() - advancedFunctionStart > 2780000)
        {
            advancedFunctionStart = 0;
            adv_searchLeft = false;
            stopDrive(0);
            playErrorSound();
        }

        if (frontMiddleDistance < 20 || frontLeftDistance < 10 || frontRightDistance < 10)
        {
            leftDrive(160);
        }
        else
        {
            advancedFunctionStart = 0;
            adv_searchLeft = false;
            stopDrive(0);
        }
    }
}

void searchRight()
{
    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
    }
    else
    {
        if (micros() - advancedFunctionStart > 2780000)
        {
            advancedFunctionStart = 0;
            adv_searchRight = false;
            stopDrive(0);
            playErrorSound();
        }

        if (frontMiddleDistance < 20 || frontLeftDistance < 10 || frontRightDistance < 10)
        {
            rightDrive(160);
        }
        else
        {
            advancedFunctionStart = 0;
            adv_searchRight = false;
            stopDrive(0);
        }
    }
}

void right360()
{
    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
    }
    else
    {
        if (micros() - advancedFunctionStart < 2780000)
        {
            rightDrive(160);
        }
        else
        {
            advancedFunctionStart = 0;
            adv_right360 = false;
            stopDrive(0);
        }
    }
}

void left360()
{
    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
    }
    else
    {
        if (micros() - advancedFunctionStart < 2780000)
        {
            leftDrive(160);
        }
        else
        {
            advancedFunctionStart = 0;
            adv_left360 = false;
            stopDrive(0);
        }
    }
}
