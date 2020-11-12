// TobyTransmit 1.0

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

int transmitData[3];
int ackData[3] = {-1, -1, -1}; // to hold the three values coming from the robot
bool newData = false;

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 300; // send every 3/10th of a second

int xAngleValue;
int yAngleValue;
int SW_PinRead;

//===============

void setup()
{

    Serial.begin(115200);
    Serial.println("TobyTransmit initiating...");

    // Set up pin for joystick switch function
    pinMode(SW_Pin, INPUT);
    digitalWrite(SW_Pin, HIGH);

    radio.begin();
    radio.setDataRate(RF24_250KBPS);

    radio.enableAckPayload();

    radio.setRetries(5, 5); // delay, count
                            // 5 gives a 1500 µsec delay which is needed for a 32 byte ackPayload
    radio.openWritingPipe(slaveAddress);

    initOLED(); // init display
}

//=============

void loop()
{
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis)
    {
        sendToToby();
        updateDisplay(); // update OLED
    }
}

//================

void sendToToby()
{
    int xPotValue = analogRead(X_PIN);
    xAngleValue = map(xPotValue, 0, 1023, 0, 180);
    int yPotValue = analogRead(Y_PIN);
    yAngleValue = map(yPotValue, 0, 1023, 0, 180);

    SW_PinRead = digitalRead(SW_Pin);

    int transmitData[3] = {xAngleValue, yAngleValue, digitalRead(SW_Pin)};

    bool rslt;
    rslt = radio.write(&transmitData, sizeof(transmitData));
    // Always use sizeof() as it gives the size as the number of bytes.
    // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent (");
    Serial.print(transmitData[0]);
    Serial.print(", ");
    Serial.print(transmitData[1]);
    Serial.print(", ");
    Serial.print(transmitData[2]);
    Serial.println(")");
    if (rslt)
    {
        if (radio.isAckPayloadAvailable())
        {
            radio.read(&ackData, sizeof(ackData));
            newData = true;
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
        Serial.println("  Tx failed");
    }

    prevMillis = millis();
}
