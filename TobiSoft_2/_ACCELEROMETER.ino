/*
The circuit:
      3V: VCC
Analog 1: x-axis
*/

#define accelerometerPinX A1 // x-axis of the accelerometer
float collisionThresholdForward = 1.35;
float collisionThresholdTurn = 1.35;

void checkForCollision()
{
    if ((DRIVE_INSTRUCTION == "STOP") || (DRIVE_INSTRUCTION == "REVERSE"))
    {
        return; // Ignore accelerometer
    }

    if (DRIVE_INSTRUCTION == "FORWARD")
    {
        int x = analogRead(accelerometerPinX); //read from xpin

        if (((((float)x - 331.5) / 65) > collisionThresholdForward)) // Collision detected!
        {
            handleCollisionEvent();
        }
    }

    if ((DRIVE_INSTRUCTION == "LEFT") || (DRIVE_INSTRUCTION == "RIGHT"))
    {
        int x = analogRead(accelerometerPinX); //read from xpin

        if (((((float)x - 331.5) / 65) > collisionThresholdTurn)) // Collision detected!
        {
            handleCollisionEvent();
        }
    }

    // DEBUG
    // Serial.print(((float)x - 331.5) / 65); //print x value on serial monitor
    // Serial.println("m/s²");
}
