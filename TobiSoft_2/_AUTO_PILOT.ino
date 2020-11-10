#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6
#define carSpeed 160
int middleDistance, leftDistance, rightDistance;
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
    if (handlingCollision)
    {
        handleCollisionEvent();
        return;
    }

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
        else if (rightDistance > leftDistance)
        {
            right();
        }
        else if (rightDistance < leftDistance)
        {
            left();
        }
    }
    else
    {
        forward();
    }
}

void handleCollisionEvent()
{
    if (collisionStart == 0)
    {
        handlingCollision = true;
        collisionStart = millis();
        stopCar();
        playTobyIntroMP3();
    }

    if (millis() - collisionStart > 2000)
    {
        reverse();
    }

    if (millis() - collisionStart > 2150)
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

    if (millis() - collisionStart > 2300)
    {
        stopCar();
    }

    if (millis() - collisionStart > 3300)
    {
        handlingCollision = false;
        collisionStart = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motor Driving Functions                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void forward()
{
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void reverse()
{
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void left()
{
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void right()
{
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void stopCar()
{
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, LOW);
}
