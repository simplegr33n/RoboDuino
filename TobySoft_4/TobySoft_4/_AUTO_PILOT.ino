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

unsigned long lastControlSwitch = 0; // for RF joystick

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
// Auto-Pilot functions                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void autoNavigation()
{

    if ((dataFromTransmitter[2] == 0) && (micros() - lastControlSwitch > 500000)) // check RF switch for control change
    {
        lastControlSwitch = micros();
        stopCar();
        displayLogo();
        AUTOPILOT_ON = false;
        return;
    }

    checkForCollision(); // Check accelerometer for a collision
    if (handlingCollision)
    {
        handleCollisionEvent();
        return;
    }

    if ((lastPilotDecision == 0) || (micros() - lastPilotDecision > 40000))
    {
        lastPilotDecision = micros();

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
        if (ultrasonicDistances[2] > ultrasonicDistances[1]) // Right v Left
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
// Manual functions                                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void manualControl()
{

    if ((dataFromTransmitter[2] == 0) && (micros() - lastControlSwitch > 500000)) // check RF switch for control change
    {
        lastControlSwitch = micros();
        stopCar();
        AUTOPILOT_ON = true;
        displayUltrasonicHeader();
        return;
    }

    if ((dataFromTransmitter[0] < 100) && (dataFromTransmitter[0] > 80) && (dataFromTransmitter[1] < 100) && (dataFromTransmitter[1] > 80))
    {
        stopCar();
        return;
    }

    if ((dataFromTransmitter[0] == -1) && (dataFromTransmitter[1] == -1))
    {
        stopCar();
        return;
    }

    int xDiff = 0;
    if (dataFromTransmitter[0] < 90)
    {
        xDiff = 90 - dataFromTransmitter[0];
    }
    else
    {
        xDiff = dataFromTransmitter[0] - 90;
    }
    int yDiff = 0;
    if (dataFromTransmitter[1] < 90)
    {
        yDiff = 90 - dataFromTransmitter[1];
    }
    else
    {
        yDiff = dataFromTransmitter[1] - 90;
    }

    if (xDiff >= yDiff) // turning takes precedence
    {
        if (dataFromTransmitter[0] < 90)
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
        if (dataFromTransmitter[1] < 90)
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
// Motor Driving functions                                                                             //
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
