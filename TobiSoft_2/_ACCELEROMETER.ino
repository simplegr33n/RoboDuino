/*
The circuit:
      3V: VCC
Analog 1: x-axis
*/

#define accelerometerPinX A1 // x-axis of the accelerometer

bool checkForCollision()
{
    int x = analogRead(accelerometerPinX); //read from xpin

    if (((((float)x - 331.5) / 65) > 1.1)) // Collision detected!
    {
        handleCollisionEvent();

        // Serial.print(((float)x - 331.5) / 65); //print x value on serial monitor
        // Serial.println("m/s²");
    }
}