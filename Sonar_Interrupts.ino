#include <stdint.h>
#include <stdbool.h>
#include <TimerOne.h>

volatile uint8_t portDstate, portDpast, changedBits;
volatile bool interruptCalled = false;

volatile unsigned long triggerTime;

const int numberOfSensors = 3;
unsigned long responseDurations[numberOfSensors];
unsigned long responseStarts[numberOfSensors] = {0, 0, 0};
int responseCount = 0; // reset to zero and trigger sensors when responseCount = numberOfSensors
volatile int pingCount = 0;

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
    if ((responseCount == numberOfSensors))
    {
        // Do not trigger, previous responses not yet dealt with
        return;
    }
    else
    {
        if ((responseCount == pingCount))
        {
            // only advance pingCount if responseCount is caught up
            pingCount++;
        }
    }

    // reset response starts (todo: loop)
    responseStarts[0] = 0;
    responseStarts[1] = 0;
    responseStarts[2] = 0;

    setTriggerPinsTo(LOW); // Set the trigger pins to LOW -- this falling edge triggers the sensor
    delayMicroseconds(10);
    setTriggerPinsTo(HIGH); // Then reset and leave HIGH -- ready for next trigger
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

    Timer1.initialize(100000);              // 500000 = half a second
    Timer1.attachInterrupt(triggerSensors); // triggerSensors to run every readingInterval
    sei();
}

void loop()
{

    if (interruptCalled)
    {
        interruptCalled = false;

        if (responseCount != numberOfSensors)
        {

            // Get durations for each sensor response (one sensor per ping for now)
            if ((pingCount == 1) && (changedBits & 0b00001000))
            {
                if (responseStarts[0] != 0)
                {
                    responseDurations[0] = (micros() - responseStarts[0]);
                    responseCount++;
                }
                else
                {
                    responseStarts[0] = micros();
                }
            }
            if ((pingCount == 2) && (changedBits & 0b00010000))
            {
                if (responseStarts[1] != 0)
                {
                    responseDurations[1] = (micros() - responseStarts[1]);
                    responseCount++;
                }
                else
                {
                    responseStarts[1] = micros();
                }
            }
            if ((pingCount == 3) && (changedBits & 0b00100000))
            {
                if (responseStarts[2] != 0)
                {
                    responseDurations[2] = (micros() - responseStarts[2]);
                    responseCount++;
                }
                else
                {
                    responseStarts[2] = micros();
                }
            }
        }
    }

    if (responseCount == numberOfSensors)
    {
        cli();

        responseCount = 0;
        pingCount = 0;

        // Do stuff
        Serial.print(microsToCM(responseDurations[0])); // left
        Serial.print("cm|");
        Serial.print(microsToCM(responseDurations[1])); // center
        Serial.print("cm|");
        Serial.print(microsToCM(responseDurations[2])); // right
        Serial.println("cm");

        sei();
    }
}

float microsToCM(long microseconds)
{
    return (float)(microseconds / 2) / 29;
}