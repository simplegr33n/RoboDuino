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

unsigned long lastRadioLink = 0;

unsigned long lastSuccessfulRadioRead = 0;

void initNRF24()
{
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();

    Serial.println(F("NRF24 online."));
}

void radioLink()
{
    if (micros() - lastRadioLink > 5000)
    {
        lastRadioLink = micros();
        cli();
        if (radio.available())
        {
            lastSuccessfulRadioRead = micros();
            radio.read(&radioJoystickAngles, sizeof(radioJoystickAngles));

            // DEBUG
            Serial.print("joystickAngles (");
            Serial.print(radioJoystickAngles[0]);
            Serial.print(", ");
            Serial.print(radioJoystickAngles[1]);
            Serial.println(")");
        }
        else
        {
            if (micros() - lastSuccessfulRadioRead > 1000000) // if longer than 1 second, set disconnected values
            {                                                 // TODO: could probably be reduced
                radioJoystickAngles[0] = -1;
                radioJoystickAngles[1] = -1;

                // DEBUG
                Serial.println("No RF Connection");
            }
        }
        sei();
    }
}
