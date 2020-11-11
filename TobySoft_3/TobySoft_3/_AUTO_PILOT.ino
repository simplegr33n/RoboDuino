#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6
#define carSpeed 160
int middleDistance, leftDistance, rightDistance;
unsigned long lastPilotDecision = 0; // micros() timestamp of last autoPilotDecision
bool handlingCollision = false;
unsigned long collisionStart = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initAutoPilot()
{
    // Set up Motor Output Pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    stopCar(); // Stop for good measure..
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Auto-Pilot Functions                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void autoNavigation()
{

    checkForCollision(); // Check accelerometer for a collision
    if (handlingCollision)
    {
        handleCollisionEvent();
        return;
    }

    if ((lastPilotDecision == 0) || (micros() - lastPilotDecision > 40000))
    {
        lastPilotDecision = micros();

        int leftDistance = microsToCentimeters(ultrasonicResponseDurations[1]);
        int middleDistance = microsToCentimeters(ultrasonicResponseDurations[0]);
        if (tofReadDistance < middleDistance) // ensure the more proximate value is used
        {
            middleDistance = tofReadDistance;
        }
        int rightDistance = microsToCentimeters(ultrasonicResponseDurations[2]);

        if ((middleDistance < 30) || (leftDistance < 25) || (rightDistance < 25))
        {
            stopCar();

            if ((rightDistance < 20) && (leftDistance < 20))
            {
                reverse();
            }
            else if (rightDistance > 20)
            {
                right();
            }
            else if (rightDistance < 20)
            {
                left();
            }
        }
        else
        {
            forward();
        }
    }
}

void handleCollisionEvent()
{
    if (collisionStart == 0)
    {
        handlingCollision = true;
        collisionStart = millis();
        stopCar();
        // todo: show something on screen
    }

    if (millis() - collisionStart > 100)
    {
        reverse();
    }

    if (millis() - collisionStart > 250)
    {
        if (ultrasonicResponseDurations[2] > ultrasonicResponseDurations[1]) // Right v Left
        {
            right();
            delay(200);
        }
        else
        {
            left();
            delay(200);
        }
    }

    if (millis() - collisionStart > 400)
    {
        stopCar();
    }

    if (millis() - collisionStart > 500)
    {
        handlingCollision = false;
        collisionStart = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Manual Functions                                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void manualControl()
{
    if ((radioJoystickAngles[0] < 100) && (radioJoystickAngles[0] > 80) && (radioJoystickAngles[1] < 100) && (radioJoystickAngles[1] > 80))
    {
        stopCar();
        return;
    }

    int xDiff = 0;
    if (radioJoystickAngles[0] < 90)
    {
        xDiff = 90 - radioJoystickAngles[0];
    }
    else
    {
        xDiff = radioJoystickAngles[0] - 90;
    }
    int yDiff = 0;
    if (radioJoystickAngles[1] < 90)
    {
        yDiff = 90 - radioJoystickAngles[1];
    }
    else
    {
        yDiff = radioJoystickAngles[1] - 90;
    }

    if (xDiff >= yDiff) // turning takes precedence
    {
        if (radioJoystickAngles[0] < 90)
        {
            left();
        }
        else
        {
            right();
        }
    }
    else
    {
        if (radioJoystickAngles[1] < 90)
        {
            forward();
        }
        else
        {
            reverse();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motor Driving Functions                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void forward()
{
    DRIVE_INSTRUCTION = "FORWARD";
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void reverse()
{
    DRIVE_INSTRUCTION = "REVERSE";
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void left()
{
    DRIVE_INSTRUCTION = "LEFT";
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void right()
{
    DRIVE_INSTRUCTION = "RIGHT";
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void stopCar()
{
    DRIVE_INSTRUCTION = "STOP";
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, LOW);
}
