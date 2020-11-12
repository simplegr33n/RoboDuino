// TobyRecieve

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 48
#define CSN_PIN 49

const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN);

unsigned long lastSuccessfulRadioRead = 0;
int ackData[3] = {0, 0, 0}; // the values to be sent to the RF Transmitter, init 0 for now

void initNRF24()
{
    Serial.println("TobySoft RF Transmit Link initiating");
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, thisSlaveAddress);

    radio.enableAckPayload();

    radio.startListening();

    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // pre-load data
}

//============
void radioLink()
{

    if (radio.available())
    {
        lastSuccessfulRadioRead = micros();
        radio.read(&dataFromTransmitter, sizeof(dataFromTransmitter));
        updateRadioReplyData();

        Serial.println(dataFromTransmitter[0]);
    }
    else
    {
        if (micros() - lastSuccessfulRadioRead > 1000000) // if no radio for more than 1 second, show disconnect values
        {
            dataFromTransmitter[0] = -1;
            dataFromTransmitter[1] = -1;
            dataFromTransmitter[2] = -1;

            // DEBUG
//            Serial.println("No RF Connection");
        }
    }
}

//================
void updateRadioReplyData()
{
    ackData[0] = tofReadDistance;                                     // most accurate front
    ackData[1] = microsToCentimeters(ultrasonicResponseDurations[1]); // left
    ackData[2] = microsToCentimeters(ultrasonicResponseDurations[2]); // right

    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time
}
