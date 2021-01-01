// L298N Motor Controller pins
#define MOTOR_ENA 5
#define MOTOR_ENB 6
#define MOTOR_IN1 3
#define MOTOR_IN2 4
#define MOTOR_IN3 7
#define MOTOR_IN4 8

#define maxSpeed 255 // Max speed (0-255)

// Advanced function variables
#define fullTurn160 3000000 // millis duration for a 360 degree turn at 160 speed
int adv_turnDegrees = 360;
bool adv_rightDegrees = false;
bool adv_leftDegrees = false;
bool adv_findOpenRight = false;
bool adv_findOpenLeft = false;
bool adv_backUpEvade = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initDriveMotors()
{
    Serial.print("Starting motors...");

    // Set up Motor Output Pins
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
    pinMode(MOTOR_ENA, OUTPUT);
    pinMode(MOTOR_ENB, OUTPUT);

    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, LOW);
    digitalWrite(MOTOR_ENA, LOW);
    digitalWrite(MOTOR_ENB, LOW);

    stopDrive(0); // Stop for good measure..
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Instruction Filter function                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void driveMotors(int direction, int percentSpeed)
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
    if (adv_backUpEvade)
    {
        backUpEvade();
        return;
    }

    // Basic Drive functions
    switch (direction)
    {
    case 0: // Stop
        stopDrive(0);
        break;
    case 1: // Forward
        forwardDrive(((float)percentSpeed / 100) * maxSpeed);
        break;
    case -1: // Reverse
        reverseDrive(( (float)percentSpeed / 100) * maxSpeed);
        break;
    case 2: // Left
        leftDrive(( (float)percentSpeed / 100) * maxSpeed);
        break;
    case 3: // Right
        rightDrive(((float) percentSpeed / 100) * maxSpeed);
        break;
    default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motor Driving functions                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void forwardDrive(int speed)
{
    DRIVE_INSTRUCTION = "FORWARD";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkForCollision())
        {
            return; // ignore basic instructions to motors if collision detected
        }

        if (checkFowardSafety())
        {
            analogWrite(MOTOR_ENA, speed);
            analogWrite(MOTOR_ENB, speed);
            digitalWrite(MOTOR_IN1, LOW);
            digitalWrite(MOTOR_IN2, HIGH);
            digitalWrite(MOTOR_IN3, HIGH);
            digitalWrite(MOTOR_IN4, LOW);
        }
    }
    else
    {
        analogWrite(MOTOR_ENA, speed);
        analogWrite(MOTOR_ENB, speed);
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
        digitalWrite(MOTOR_IN3, HIGH);
        digitalWrite(MOTOR_IN4, LOW);
    }
}

void reverseDrive(int speed)
{
    DRIVE_INSTRUCTION = "REVERSE";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkForCollision())
        {
            return; // ignore basic instructions to motors if collision detected
        }

        if (checkReverseSafety())
        {
            analogWrite(MOTOR_ENA, speed);
            analogWrite(MOTOR_ENB, speed);
            digitalWrite(MOTOR_IN1, HIGH);
            digitalWrite(MOTOR_IN2, LOW);
            digitalWrite(MOTOR_IN3, LOW);
            digitalWrite(MOTOR_IN4, HIGH);
        }
    }
    else
    {
        analogWrite(MOTOR_ENA, speed);
        analogWrite(MOTOR_ENB, speed);
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        digitalWrite(MOTOR_IN3, LOW);
        digitalWrite(MOTOR_IN4, HIGH);
    }
}

void leftDrive(int speed)
{
    DRIVE_INSTRUCTION = "LEFT";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkForCollision())
        {
            return; // ignore basic instructions to motors if collision detected
        }

        if (checkLeftSafety())
        {
            analogWrite(MOTOR_ENA, speed);
            analogWrite(MOTOR_ENB, speed);
            digitalWrite(MOTOR_IN1, HIGH);
            digitalWrite(MOTOR_IN2, LOW);
            digitalWrite(MOTOR_IN3, HIGH);
            digitalWrite(MOTOR_IN4, LOW);
        }
    }
    else
    {
        analogWrite(MOTOR_ENA, speed);
        analogWrite(MOTOR_ENB, speed);
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        digitalWrite(MOTOR_IN3, HIGH);
        digitalWrite(MOTOR_IN4, LOW);
    }
}

void rightDrive(int speed)
{
    DRIVE_INSTRUCTION = "RIGHT";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkForCollision())
        {
            return; // ignore basic instructions to motors if collision detected
        }

        if (checkRightSafety())
        {
            analogWrite(MOTOR_ENA, speed);
            analogWrite(MOTOR_ENB, speed);
            digitalWrite(MOTOR_IN1, LOW);
            digitalWrite(MOTOR_IN2, HIGH);
            digitalWrite(MOTOR_IN3, LOW);
            digitalWrite(MOTOR_IN4, HIGH);
        }
    }
    else
    {

        analogWrite(MOTOR_ENA, speed);
        analogWrite(MOTOR_ENB, speed);
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
        digitalWrite(MOTOR_IN3, LOW);
        digitalWrite(MOTOR_IN4, HIGH);
    }
}

void stopDrive(int speed) // TODO: speed argument applies a small reverse/delay() if not 0, in case of sudden stop at dropoff
{
    DRIVE_INSTRUCTION = "STOP";

    if (AUTOPILOT_ON || SAFEMODE_ON)
    {
        if (checkForCollision())
        {
            return; // ignore basic instructions to motors if collision detected
        }

        if (checkStopSafety())
        {
            digitalWrite(MOTOR_ENA, LOW);
            digitalWrite(MOTOR_ENB, LOW);
        }
    }
    else
    {
        digitalWrite(MOTOR_ENA, LOW);
        digitalWrite(MOTOR_ENB, LOW);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Advanced Driving functions                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void leftDegrees(int degrees)
{
    if ((AUTOPILOT_ON || SAFEMODE_ON) && (checkLeftSafety() == false))
    {
        clearAdvancedFunctions();
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
        evadeTryCount++;
        clearAdvancedFunctions();
    }

    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_findOpenLeft = true;
    }
    else
    {
        if (frontCenterDistance < 20 || frontLeftDistance < 10 || frontRightDistance < 10)
        {
            if (micros() - advancedFunctionStart < fullTurn160 / 6)
            {
                leftDrive(autoPilotSpeed * 3);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 5)
            {
                stopDrive(0);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 4)
            {
                leftDrive(autoPilotSpeed * 3);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 3)
            {
                stopDrive(0);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 2)
            {
                leftDrive(autoPilotSpeed * 3);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 1)
            {
                stopDrive(0);
                return;
            }
            else if (micros() - advancedFunctionStart < (fullTurn160 + (fullTurn160 / 6)))
            {
                leftDrive(autoPilotSpeed * 3);
                return;
            }
            else if (micros() - advancedFunctionStart < (fullTurn160 + (fullTurn160 / 5)))
            {
                stopDrive(0);
                return;
            }
            else if (micros() - advancedFunctionStart < (fullTurn160 + (fullTurn160 / 4)))
            {
                leftDrive(autoPilotSpeed * 3);
                return;
            }
        }

        evadeTryCount++;
        clearAdvancedFunctions();
    }
}

void findClearRight()
{
    if ((AUTOPILOT_ON || SAFEMODE_ON) && (checkRightSafety() == false))
    {
        evadeTryCount++;
        clearAdvancedFunctions();
    }

    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_findOpenRight = true;
    }
    else
    {
        if (frontCenterDistance < 20 || frontLeftDistance < 10 || frontRightDistance < 10)
        {
            if (micros() - advancedFunctionStart < fullTurn160 / 6)
            {
                rightDrive(autoPilotSpeed * 3);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 5)
            {
                stopDrive(0);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 4)
            {
                rightDrive(autoPilotSpeed * 3);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 3)
            {
                stopDrive(0);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 2)
            {
                rightDrive(autoPilotSpeed * 3);
                return;
            }
            else if (micros() - advancedFunctionStart < fullTurn160 / 1)
            {
                stopDrive(0);
                return;
            }
            else if (micros() - advancedFunctionStart < (fullTurn160 + (fullTurn160 / 6)))
            {
                rightDrive(autoPilotSpeed * 3);
                return;
            }
            else if (micros() - advancedFunctionStart < (fullTurn160 + (fullTurn160 / 5)))
            {
                stopDrive(0);
                return;
            }
            else if (micros() - advancedFunctionStart < (fullTurn160 + (fullTurn160 / 4)))
            {
                rightDrive(autoPilotSpeed * 3);
                return;
            }
        }

        evadeTryCount++;
        clearAdvancedFunctions();
    }
}

void backUpEvade()
{
    if (advancedFunctionStart == 0)
    {
        advancedFunctionStart = micros();
        adv_backUpEvade = true;
    }
    else
    {
        if (micros() - advancedFunctionStart < 200000)
        {
            reverseDrive(autoPilotSpeed * 3);
            return;
        }
        else if (micros() - advancedFunctionStart < 500000)
        {
            rightDrive(autoPilotSpeed * 3);
            return;
        }
        else if (micros() - advancedFunctionStart < 700000)
        {
            reverseDrive(autoPilotSpeed * 3);
            return;
        }
        else if (micros() - advancedFunctionStart < 1000000)
        {
            rightDrive(autoPilotSpeed * 3);
            return;
        }
        else if (micros() - advancedFunctionStart < 1200000)
        {
            reverseDrive(autoPilotSpeed * 3);
            return;
        }
        else if (micros() - advancedFunctionStart < 1700000)
        {
            rightDrive(autoPilotSpeed * 3);
            return;
        }
        else if (micros() - advancedFunctionStart < 2000000)
        {
            reverseDrive(autoPilotSpeed * 3);
            return;
        }
        else if (micros() - advancedFunctionStart < 2300000)
        {
            rightDrive(autoPilotSpeed * 3);
            return;
        }

        evadeTryCount++;
        clearAdvancedFunctions();
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
    adv_backUpEvade = false;
}
