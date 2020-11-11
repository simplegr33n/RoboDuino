/*
* NRF24L01 Joystick
*   Transmitter Code
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Analog Stick connections
// XPot - A0 // YPot - A1 // SW - D2

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
int returnAngles = 0;

void setup()
{
    Serial.begin(115200);

    radio.begin();
    radio.openWritingPipe(addresses[1]);    // 00002
    radio.openReadingPipe(1, addresses[0]); // 00001
    radio.setPALevel(RF24_PA_MIN);
}

void loop()
{
    delay(5); // TODO: deal with (though not the hugest issue in a dedicated transmitter)

    radio.stopListening();

    int xPotValue = analogRead(A0);
    int xAngleValue = map(xPotValue, 0, 1023, 0, 180);
    int yPotValue = analogRead(A1);
    int yAngleValue = map(yPotValue, 0, 1023, 0, 180);

    Serial.println("Send X");
    Serial.println(xAngleValue);
    Serial.println("Send Y");
    Serial.println(yAngleValue);

    int joystickAngles[2] = {xAngleValue, yAngleValue};

    radio.write(&joystickAngles, sizeof(joystickAngles));

    delay(5); // TODO: deal with (though not the hugest issue in a dedicated transmitter)

    radio.startListening();
    while (!radio.available())
        ;
    radio.read(&returnAngles, sizeof(returnAngles));

    Serial.println("Return angles");
    Serial.println(returnAngles);
}
