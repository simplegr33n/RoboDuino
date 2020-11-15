#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 48
#define CSN_PIN 49

RF24 radio(CE_PIN, CSN_PIN); // create a radio

const byte thisSlaveAddress[5] = {'R', 'x', 'T', 'B', '0'};

unsigned long lastSuccessfulRadioRead = 0;
int ackData[3] = {0, 0, 0}; // the values to be sent to the RF Transmitter, init 0 for now

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Receive / Acknowledge functions                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void radioLink()
{

    if (radio.available())
    {
        lastSuccessfulRadioRead = micros();
        radio.read(&dataFromTransmitter, sizeof(dataFromTransmitter));
        updateRadioReplyData();

        Serial.println(dataFromTransmitter[0]);

        // // DEBUG
        // Serial.println("data received (");
        // Serial.print(dataFromTransmitter[0]);
        // Serial.print(", ");
        // Serial.print(dataFromTransmitter[1]);
        // Serial.print(", ");
        // Serial.print(dataFromTransmitter[2]);
        // Serial.println(")");
    }
    else
    {
        if (micros() - lastSuccessfulRadioRead > 1000000) // if no radio for more than 1 second, show disconnect values
        {
            dataFromTransmitter[0] = -1;
            dataFromTransmitter[1] = -1;
            dataFromTransmitter[2] = -1;

            // // DEBUG
            // Serial.println("No RF Connection");
        }
    }
}

void updateRadioReplyData()
{

    // Get best Front-Left distance
    int leftDistance = ultrasonicDistances[1];
    if (irProxValueFL == 0)
    {
        leftDistance = 1;
    }

    // Get best Front-Center distance
    int middleDistance = ultrasonicDistances[0];
    if (tofReadDistance < middleDistance)
    {
        middleDistance = tofReadDistance;
    }
    if (irProxValueFC == 0)
    {
        middleDistance = 1;
    }

    // Get best Front-Right distance
    int rightDistance = ultrasonicDistances[2];
    if (irProxValueFR == 0)
    {
        rightDistance = 1;
    }

    ackData[0] = middleDistance;
    ackData[1] = leftDistance;
    ackData[2] = rightDistance;

    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time
}
