#include <stdint.h>
#include <stdbool.h>
#include <TimerOne.h>

volatile uint8_t portDstate, portDpast, changedBitsD;
volatile bool ultrasonicInterruptCalled = false;

volatile unsigned long triggerTime;

const int ultrasonicSensorQuantity = 3;
unsigned long ultrasonicResponseDurations[ultrasonicSensorQuantity];
unsigned long ultrasonicResponseStarts[ultrasonicSensorQuantity] = {0, 0, 0};
int ultrasonicResponseCount = 0; // reset to zero and trigger sensors when responseCount = numberOfSensors
volatile int ultrasonicPingCount = 0;

ISR(PCINT2_vect)
{
    portDpast = portDstate;
    portDstate = PIND; // get state of Port D with PIND
    changedBitsD = portDpast ^ portDstate;
    ultrasonicInterruptCalled = true;
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
    if ((ultrasonicResponseCount == ultrasonicSensorQuantity))
    {
        // Do not trigger, previous responses not yet dealt with
        return;
    }
    else
    {
        if ((ultrasonicResponseCount == ultrasonicPingCount))
        {
            // only advance pingCount if responseCount is caught up
            ultrasonicPingCount++;
        }
    }

    // reset response starts (todo: loop)
    ultrasonicResponseStarts[0] = 0;
    ultrasonicResponseStarts[1] = 0;
    ultrasonicResponseStarts[2] = 0;

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

    Timer1.initialize(50000);               // 500000 = half a second
    Timer1.attachInterrupt(triggerSensors); // triggerSensors to run every readingInterval
    sei();
}

void loop()
{

    if (ultrasonicInterruptCalled)
    {
        ultrasonicInterruptCalled = false;

        if (ultrasonicResponseCount != ultrasonicSensorQuantity)
        {

            // Get durations for each sensor response (one sensor per ping for now)
            if ((ultrasonicPingCount == 1) && (changedBitsD & 0b00001000))
            {
                if (ultrasonicResponseStarts[0] != 0)
                {
                    ultrasonicResponseDurations[0] = (micros() - ultrasonicResponseStarts[0]);
                    ultrasonicResponseCount++;
                }
                else
                {
                    ultrasonicResponseStarts[0] = micros();
                }
            }
            if ((ultrasonicPingCount == 2) && (changedBitsD & 0b00010000))
            {
                if (ultrasonicResponseStarts[1] != 0)
                {
                    ultrasonicResponseDurations[1] = (micros() - ultrasonicResponseStarts[1]);
                    ultrasonicResponseCount++;
                }
                else
                {
                    ultrasonicResponseStarts[1] = micros();
                }
            }
            if ((ultrasonicPingCount == 3) && (changedBitsD & 0b00100000))
            {
                if (ultrasonicResponseStarts[2] != 0)
                {
                    ultrasonicResponseDurations[2] = (micros() - ultrasonicResponseStarts[2]);
                    ultrasonicResponseCount++;
                }
                else
                {
                    ultrasonicResponseStarts[2] = micros();
                }
            }
        }
    }

    if (ultrasonicResponseCount == ultrasonicSensorQuantity)
    {
        cli();

        ultrasonicResponseCount = 0;
        ultrasonicPingCount = 0;

        // Do stuff
        Serial.print(microsToCM(ultrasonicResponseDurations[0])); // left
        Serial.print("cm|");
        Serial.print(microsToCM(ultrasonicResponseDurations[1])); // center
        Serial.print("cm|");
        Serial.print(microsToCM(ultrasonicResponseDurations[2])); // right
        Serial.println("cm");

        sei();
    }
}

float microsToCM(long microseconds)
{
    return (float)(microseconds / 2) / 29;
}