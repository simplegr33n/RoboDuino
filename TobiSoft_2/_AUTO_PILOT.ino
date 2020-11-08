#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6
#define carSpeed 160
int middleDistance, leftDistance, rightDistance;

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
    stop(); // Stop for good measure..
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Auto-Pilot Functions                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void autoNavigation()
{
    int leftDistance = microsToCentimeters(ultrasonicResponseDurations[0]);
    int middleDistance = microsToCentimeters(ultrasonicResponseDurations[1]);
    if (tofReadDistance < middleDistance) // ensure the more proximate value is used
    {
        middleDistance = tofReadDistance;
    }
    int rightDistance = microsToCentimeters(ultrasonicResponseDurations[2]);

    if ((middleDistance < 30) || (leftDistance < 25) || (rightDistance < 25))
    {
        stop();

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

void stop()
{
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, LOW);
}
