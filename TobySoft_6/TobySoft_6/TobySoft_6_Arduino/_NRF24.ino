#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN); // create a radio

const byte thisSlaveAddress[5] = {'R', 'x', 'T', 'B', '0'};

unsigned long lastSuccessfulRadioRead = 0;
int ackData[4] = {0, 0, 0, 0}; // the values to be sent to the RF Transmitter, init 0 for now

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
    Serial.println("TobySoft RF Transmit Link established");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Receive / Acknowledge functions                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void radioLink()
{

    if (radio.available())
    {
        radio.read(&dataFromTransmitter, sizeof(dataFromTransmitter));

        if ((dataFromTransmitter[0] == 0) && (dataFromTransmitter[1] == 0) && (dataFromTransmitter[2] == 0) && (dataFromTransmitter[3] == 0))
        {
            dataFromTransmitter[0] = -1;
            dataFromTransmitter[1] = -1;
            dataFromTransmitter[2] = -1;
            dataFromTransmitter[3] = -1;

            // DEBUG
//            Serial.println("No RF Connection");
        }
        else
        {
            lastSuccessfulRadioRead = millis();
            updateRadioReplyData();

            // DEBUG
//            Serial.print("data received (");
//            Serial.print(dataFromTransmitter[0]);
//            Serial.print(", ");
//            Serial.print(dataFromTransmitter[1]);
//            Serial.print(", ");
//            Serial.print(dataFromTransmitter[2]);
//            Serial.print(", ");
//            Serial.print(dataFromTransmitter[3]);
//            Serial.println(")");
        }
    }
    else
    {
        if (millis() - lastSuccessfulRadioRead > 500) // if no radio for more than 1/2 second, show disconnect values
        {
            dataFromTransmitter[0] = -1;
            dataFromTransmitter[1] = -1;
            dataFromTransmitter[2] = -1;
            dataFromTransmitter[3] = -1;

            // DEBUG
//            Serial.println("No RF Connection");
        }
    }
}

void updateRadioReplyData()
{
    // Get robotState
    int robotState = 0;
    if (SAFEMODE_ON)
    {
        robotState = 1;
    }
    if (AUTOPILOT_ON)
    {
        robotState = 2;
    }

    ackData[0] = robotState;
    ackData[1] = frontCenterDistance;
    ackData[2] = frontLeftDistance;
    ackData[3] = frontRightDistance;

    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time
}
