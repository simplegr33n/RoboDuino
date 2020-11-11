/*
* NRF24L01 Joystick
*   Receiver Code
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(48, 49); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup()
{
    Serial.begin(115200);

    radio.begin();
    radio.openWritingPipe(addresses[0]);    // 00001
    radio.openReadingPipe(1, addresses[1]); // 00002
    radio.setPALevel(RF24_PA_MIN);

    Serial.println("startup");
}

void loop()
{
    makeRadioLink();
}

void makeRadioLink()
{
    delay(5); // TODO: deal with (especially important on Rx side, no delays in car loop)

    radio.startListening();
    if (radio.available())
    {
        int radioJoystickAngles[2];

        while (radio.available())
        {
            radio.read(&radioJoystickAngles, sizeof(radioJoystickAngles));

            Serial.println("joystickAngles");
            Serial.println(radioJoystickAngles[0]);
            Serial.println(radioJoystickAngles[1]);
        }

        delay(5); // TODO: deal with (especially important on Rx side, no delays in car loop)

        radio.stopListening();

        radio.write(&radioJoystickAngles, sizeof(radioJoystickAngles));
    }
}
