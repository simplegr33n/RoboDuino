#include <stdint.h>
#include <stdbool.h>
#include <TimerOne.h>

volatile uint8_t portDstate, portDpast, changedBits;
volatile bool interruptCalled = false;

volatile unsigned long triggerTime;

bool D3_isReading, D4_isReading, D5_isReading;
volatile int responseDuration[3];
const int numberOfSensors = 3;
int responseCount = 0; // reset to zero and trigger sensors when responseCount = numberOfSensors

ISR(PCINT2_vect)
{
    portDpast = portDstate;
    portDstate = PIND; // get state of Port D with PIND
    changedBits = portDpast ^ portDstate;
    interruptCalled = true;
}

void setTriggerPinsTo(uint8_t signalState)
{
    switch (signalState)
    {
    case LOW:
        PORTD &= ~(1UL << 2); // Set trigger pin D3 LOW
        break;
    case HIGH:
        // Port D handles D0 to D7
        PORTD |= 1UL << 2; // Set trigger pin D3 HIGH
        break;
    default:
        break;
    }
}

void triggerSensors(void)
{
    if (responseCount == numberOfSensors)
    {
        Serial.println(responseCount);
        return;
    }
    // Set the trigger pins back to LOW -- this falling edge triggers the sensor
    setTriggerPinsTo(LOW);
    triggerTime = micros();
    delayMicroseconds(10);
    setTriggerPinsTo(HIGH);
}

void setup()
{
    cli();
    DDRD = 0b00000100;      // set all bits in Port B Data Direction Register to input, except PCINT18 (our trigger)
    setTriggerPinsTo(HIGH); // ensure trigger pins at high before enabling interrupts
    portDstate = PIND;
    Serial.begin(9600);
    PCICR |= (1 << PCIE2);                                      // Pin Change Interrupt Control Register enabling Port B
    PCMSK2 |= (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); // Enable mask on PCINT19 to trigger interupt on state change

    Timer1.initialize(200000);              // 500000 = half a second
    Timer1.attachInterrupt(triggerSensors); // triggerSensors to run every readingInterval
    sei();
}

void loop()
{
    if (interruptCalled)
    {
        interruptCalled = false;

        // Get durations for each sensor response (one sensor per ping for now)
        if ((changedBits & 0b00001000) && (responseCount == 0))
        {
            if (D3_isReading)
            {
                responseDuration[1] = (micros() - triggerTime);
                responseCount++;
                D3_isReading = false;
            }
            else
            {
                D3_isReading = true;
            }
        }
        if ((changedBits & 0b00010000) && (responseCount == (numberOfSensors - 2)))
        {
            if (D4_isReading)
            {
                responseDuration[0] = (micros() - triggerTime);
                responseCount++;
                D4_isReading = false;
            }
            else
            {
                D4_isReading = true;
            }
        }
        if ((changedBits & 0b00100000) && (responseCount == (numberOfSensors - 1)))
        {
            if (D5_isReading)
            {
                responseDuration[2] = (micros() - triggerTime);
                responseCount++;
                D5_isReading = false;
            }
            else
            {
                D5_isReading = true;
            }
        }
    }

    if (responseCount == numberOfSensors)
    {
        // Do stuff
        Serial.print(microsToCM(responseDuration[0])); // left
        Serial.print("cm[]");
        Serial.print(microsToCM(responseDuration[1])); // center
        Serial.print("cm[]");
        Serial.print(microsToCM(responseDuration[2])); // right
        Serial.println("cm[]");

        responseCount = 0;
        responseDuration[0] = 0;
        responseDuration[1] = 0;
        responseDuration[2] = 0;
    }
}

float microsToCM(long microseconds)
{
    return (float)(microseconds / 2) / 29;
}