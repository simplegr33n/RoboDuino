/*
* NRF24L01 Joystick
*   Receiver Code
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 48
#define CSN_PIN 49

const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN);

int joystickAngles[2]; // for receive data

void setup()
{
    Serial.begin(9600);
  
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();

    Serial.println(F("NRF24 online."));
}

void loop()
{
    radioLink();
}

void radioLink()
{
    if (radio.available())
    {
        radio.read(&joystickAngles, sizeof(joystickAngles));
        Serial.println("joystickAngles");
        Serial.println(joystickAngles[0]);
        Serial.println(joystickAngles[1]);
    }
}
