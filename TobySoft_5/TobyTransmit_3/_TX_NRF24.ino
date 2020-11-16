#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

const byte slaveAddress[5] = {'R', 'x', 'T', 'B', '0'};

unsigned long prevMillis = 0;
unsigned long txIntervalMillis = 50; // send every 1/10th of a second

int ackData[4] = {-1, -1, -1, -1}; // values from robot (robotState, midDist, leftDist, rightDist)

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initNRF24()
{
    Serial.println("TobyTransmit initiating...");

    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.enableAckPayload();
    radio.setRetries(5, 5); // delay, count (5 gives a 1500 µsec delay which is needed for a 32 byte ackPayload)
    radio.openWritingPipe(slaveAddress);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Transmit functions                                                                                  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void sendRadioCom()
{
    if (millis() - prevMillis > txIntervalMillis)
    {
        prevMillis = millis();
        sendToToby();
        updateDisplay(); // update OLED
    }
}

void sendToToby()
{
    int transmitData[4] = {joystick0ValueX, joystick0ValueY, joystick0ValueSw, bigRedButtonValue}; // needs to match datatype/size set up to be received on other end

    bool rslt;
    rslt = radio.write(&transmitData, sizeof(transmitData)); // !always use sizeof() as it gives the size as the correct number of bytes

    if (rslt) // check for acknowledgement
    {
        if (radio.isAckPayloadAvailable())
        {
            radio.read(&ackData, sizeof(ackData));
        }
        else
        {
            Serial.println("  Acknowledge but no data ");
        }
    }
    else
    {
        ackData[0] = -1; // set disconnected values
        ackData[1] = -1;
        ackData[2] = -1;
        ackData[3] = -1;
        Serial.println("  Tx failed");
    }

    // // DEBUG
    // Serial.print("Data Sent (");
    // Serial.print(transmitData[0]);
    // Serial.print(", ");
    // Serial.print(transmitData[1]);
    // Serial.print(", ");
    // Serial.print(transmitData[2]);
    // Serial.println(")");
}
