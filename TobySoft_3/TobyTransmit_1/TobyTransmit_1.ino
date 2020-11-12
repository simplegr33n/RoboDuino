// TobySoft Transmitter (For remote RF control)
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// NRF24L01
#define CE_PIN 7
#define CSN_PIN 8

// Joystick
#define X_PIN A0
#define Y_PIN A1
#define SW_Pin 2

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int radioJoystickAngles[2];

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 250; // send four times per second

void setup()
{
    Serial.begin(115200);

    Serial.println("TobySoft Transmitter Starting");

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

    int xPotValue = analogRead(X_PIN);
    int xAngleValue = map(xPotValue, 0, 1023, 0, 180);
    int yPotValue = analogRead(Y_PIN);
    int yAngleValue = map(yPotValue, 0, 1023, 0, 180);

    // // DEBUG
    //     Serial.println("Send X");
    //     Serial.println(xAngleValue);
    //     Serial.println("Send Y");
    //     Serial.println(yAngleValue);

    Serial.print("SW!!: ");
    Serial.println(digitalRead(SW_Pin));

    int radioJoystickAngles[2] = {xAngleValue, yAngleValue};

    bool rslt;
    rslt = radio.write(&radioJoystickAngles, sizeof(radioJoystickAngles));
    // Always use sizeof() as it gives the size as the number of bytes.
    // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent (");
    Serial.print(radioJoystickAngles[0]);
    Serial.print(",");
    Serial.print(radioJoystickAngles[1]);
    Serial.println(")");
    if (rslt)
    {
        Serial.println("  Acknowledge received");
    }
    else
    {
        Serial.println("  Tx failed");
    }
}
