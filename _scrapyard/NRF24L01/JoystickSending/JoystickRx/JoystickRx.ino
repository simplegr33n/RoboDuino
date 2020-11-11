/*
* NRF24L01 Joystick
*   Receiver Code
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup()
{
    Serial.begin(9600);

    radio.begin();
    radio.openWritingPipe(addresses[0]);    // 00001
    radio.openReadingPipe(1, addresses[1]); // 00002
    radio.setPALevel(RF24_PA_MIN);
}

void loop()
{
    delay(5); // TODO: deal with (especially important on Rx side, no delays in car loop)

    radio.startListening();
    if (radio.available())
    {
        int joystickAngles[2];

        while (radio.available())
        {
            radio.read(&joystickAngles, sizeof(joystickAngles));

            Serial.println("joystickAngles");
            Serial.println(joystickAngles[0]);
            Serial.println(joystickAngles[1]);
        }

        delay(5); // TODO: deal with (especially important on Rx side, no delays in car loop)

        radio.stopListening();

        radio.write(&joystickAngles, sizeof(joystickAngles));
    }
}