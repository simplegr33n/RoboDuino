// TobyRecieve

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 48
#define CSN_PIN 49

const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN);

int receiveData[3];           // this must match transmitData in the TX
int ackData[2] = {109, -4000}; // the two values to be sent to the master
bool newData = false;

//==============

void setup()
{

    Serial.begin(115200);

    Serial.println("TobySoft RF Transmit Link initiating");
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, thisSlaveAddress);

    radio.enableAckPayload();

    radio.startListening();

    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // pre-load data
}

//==========

void loop()
{
    getData();
    showData();
}

//============

void getData()
{
    if (radio.available())
    {
        radio.read(&receiveData, sizeof(receiveData));
        updateReplyData();
        newData = true;
    }
}

//================

void showData()
{
    if (newData == true)
    {
        Serial.print("Data received (");
        Serial.print(receiveData[0]);
        Serial.print(", ");
        Serial.print(receiveData[1]);
        Serial.print(", ");
                Serial.print(receiveData[2]);
                Serial.print(")");
        Serial.print(" ackPayload sent ");
        Serial.print(ackData[0]);
        Serial.print(", ");
        Serial.println(ackData[1]);
        newData = false;
    }
}

//================

void updateReplyData()
{
    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time
}
