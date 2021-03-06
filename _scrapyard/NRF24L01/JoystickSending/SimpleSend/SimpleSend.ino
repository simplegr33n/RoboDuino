// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN A2
#define CSN_PIN A3

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int radioJoystickAngles[2];

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second

void setup()
{
    Serial.begin(9600);

    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setRetries(3, 5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

//====================

void loop()
{
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis)
    {
        send();
        prevMillis = millis();
    }
}

//====================

void send()
{

    int xPotValue = analogRead(A0);
    int xAngleValue = map(xPotValue, 0, 1023, 0, 180);
    int yPotValue = analogRead(A1);
    int yAngleValue = map(yPotValue, 0, 1023, 0, 180);

    Serial.println("Send X");
    Serial.println(xAngleValue);
    Serial.println("Send Y");
    Serial.println(yAngleValue);

    int radioJoystickAngles[2] = {xAngleValue, yAngleValue};

    bool rslt;
    rslt = radio.write(&radioJoystickAngles, sizeof(radioJoystickAngles));
    // Always use sizeof() as it gives the size as the number of bytes.
    // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    Serial.print(radioJoystickAngles[0]);
    Serial.print(radioJoystickAngles[1]);
    if (rslt)
    {
        Serial.println("  Acknowledge received");
    }
    else
    {
        Serial.println("  Tx failed");
    }
}
