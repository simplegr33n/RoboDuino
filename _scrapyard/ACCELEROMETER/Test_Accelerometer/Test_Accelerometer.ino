/*
ADXL335
note:vcc-->5v ,but ADXL335 Vs is 3.3V
The circuit:
      5V: VCC
analog 1: x-axis
analog 2: y-axis
analog 3: z-axis
*/

#define accelerometerPinX A1 // x-axis of the accelerometer

void setup()
{
    // initialize the serial communications:
    Serial.begin(9600);
}

void loop()
{
    int x = analogRead(accelerometerPinX); //read from xpin

    // if (((((float)x - 331.5) / 65) > 1.8) || ((((float)x - 331.5) / 65) < -0.2)) // Collision detected!
    if (((((float)x - 331.5) / 65) > 1.1)) // Collision detected!
    {

        Serial.print(((float)x - 331.5) / 65); //print x value on serial monitor
        Serial.println("m/s²");
    }

    delay(1);
}