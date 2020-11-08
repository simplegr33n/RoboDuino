#include <stdint.h>
#include <stdbool.h>
#include <TimerOne.h>

// Port B
volatile uint8_t portKstate, portKpast, changedBitsK;
volatile bool ultrasonicInterruptCalled = false;

volatile unsigned long triggerTime;

const int ultrasonicSensorQuantity = 3;
unsigned long ultrasonicResponseDurations[ultrasonicSensorQuantity];
unsigned long ultrasonicResponseStarts[ultrasonicSensorQuantity] = {0, 0, 0};
int ultrasonicResponseCount = 0; // reset to zero and trigger sensors when responseCount = numberOfSensors
volatile int ultrasonicPingCount = 0;

ISR(PCINT2_vect)
{
    portKpast = portKstate;
    portKstate = PINK; // get state of Port K with PINB
    changedBitsK = portKpast ^ portKstate;
    ultrasonicInterruptCalled = true;
}

void setTriggerPinsTo(uint8_t signalState)
{
    switch (signalState)
    {
    case LOW:
        PORTB &= ~(1UL << 4); // Set trigger pin D10 LOW
        break;
    case HIGH:
        PORTB |= 1UL << 4; // Set trigger pin D10 HIGH
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
    DDRB = 0b00010000;      // set all bits in Port B Data Direction Register to input, except PCINT18 (our trigger)
    setTriggerPinsTo(HIGH); // ensure trigger pins at high before enabling interrupts
    portKstate = PINK;      // Pin - K.. lol
    Serial.begin(9600);
    PCICR |= (1 << PCIE2);                                      // Pin Change Interrupt Control Register enabling Port K
    PCMSK2 |= (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23); // Enable mask on PCINT21-23 to trigger interrupt on state change

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
            if ((ultrasonicPingCount == 1) && (changedBitsK & 0b00100000)) // Pin Change Mask Register 2 location for Center (Pin A13)
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
            if ((ultrasonicPingCount == 2) && (changedBitsK & 0b01000000)) // Pin Change Mask Register 2 location for Right (Pin A14)
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
            if ((ultrasonicPingCount == 3) && (changedBitsK & 0b10000000)) // Pin Change Mask Register 2 location for Left (Pin A15)
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
        Serial.print(microsToCentimeters(ultrasonicResponseDurations[0])); // left
        Serial.print("cm|");
        Serial.print(microsToCentimeters(ultrasonicResponseDurations[1])); // center
        Serial.print("cm|");
        Serial.print(microsToCentimeters(ultrasonicResponseDurations[2])); // right
        Serial.println("cm");

        sei();
    }
}

float microsToCentimeters(long microseconds)
{
    return (float)(microseconds / 2) / 29;
}
