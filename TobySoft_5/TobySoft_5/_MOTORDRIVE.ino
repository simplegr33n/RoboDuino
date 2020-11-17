// L298N Motor Controller pins
#define MOTOR_ENB 5
#define MOTOR_IN1 7
#define MOTOR_IN2 8
#define MOTOR_IN3 9
#define MOTOR_IN4 11
#define MOTOR_ENA 6

#define carSpeed 140

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
    stopMotors(0); // Stop for good measure..
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

        switch (direction)
        {
        case 0: // Stop
            if (checkStopSafety())
            {
                stopMotors(0);
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
            stopMotors(0);
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

void stopMotors(int speed) // TODO: speed argument applies a small reverse/delay() if not 0, in case of sudden stop at dropoff
{
    DRIVE_INSTRUCTION = "STOP";
    digitalWrite(MOTOR_ENA, LOW);
    digitalWrite(MOTOR_ENB, LOW);
}
