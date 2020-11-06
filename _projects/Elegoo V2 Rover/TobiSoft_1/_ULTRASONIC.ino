#include <TimerOne.h>

#define ULTRASONIC_INTERVAL 80000 // needs some tuning

unsigned long ultrasonicResponseStarts[ultrasonicSensorQuantity];
volatile int ultrasonicPingCount = 0;

void initUltrasonicInterrupts()
{
    // Initialize Ping Interrupts
    cli();
    DDRB = 0b00000100;      // Set our trigger pin (D10) to output
    setTriggerPinsTo(HIGH); // Ensure trigger pins at high before enabling interrupts
    DDRD = 0b00000000;      // Set all bits in Port D Data Direction Register to input
    portDstate = PIND;

    PCICR |= (1 << PCIE2);                                      // Pin Change Interrupt Control Register enabling Port D
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20); // Enable mask on PCINT18-20 to trigger interrupt on state change

    Timer1.initialize(ULTRASONIC_INTERVAL); // (500000 = half a second, 30000 perhaps minimum)
    Timer1.attachInterrupt(triggerSensors); // TriggerSensors to run every readingInterval
    sei();
}

ISR(PCINT2_vect)
{
    portDpast = portDstate;
    portDstate = PIND; // Get state of Port D with PIND
    changedBits = portDpast ^ portDstate;
    ultrasonicInterruptCalled = true;
}

void resolveUltrasonicInterrupt()
{

    ultrasonicInterruptCalled = false;

    if (ultrasonicResponseCount != ultrasonicSensorQuantity)
    {

        // Get durations for each sensor response (one sensor per ping for now)
        if ((ultrasonicPingCount == 1) && (changedBits & 0b00000100))
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
        if ((ultrasonicPingCount == 2) && (changedBits & 0b00001000))
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
        if ((ultrasonicPingCount == 3) && (changedBits & 0b00010000))
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

void updateUltrasonicHistory()
{
    cli();
    ultrasonicResponseCount = 0;
    ultrasonicPingCount = 0;

    D2_history[ultrasonicHistoryPointer] = microsToCM(ultrasonicResponseDurations[0]);
    D3_history[ultrasonicHistoryPointer] = microsToCM(ultrasonicResponseDurations[1]);
    D4_history[ultrasonicHistoryPointer] = microsToCM(ultrasonicResponseDurations[2]);

    // Update pointer
    ultrasonicHistoryPointer++;
    if (ultrasonicHistoryPointer > 7)
    {
        ultrasonicHistoryPointer = 0;
    }
    sei();

    // DEBUG
    //    Serial.print(microsToCM(ultrasonicResponseDurations[0])); // left
    //    Serial.print("cm|");
    //    Serial.print(microsToCM(ultrasonicResponseDurations[1])); // center
    //    Serial.print("cm|");
    //    Serial.print(microsToCM(ultrasonicResponseDurations[2])); // right
    //    Serial.println("cm");
}

void setTriggerPinsTo(uint8_t signalState)
{
    switch (signalState)
    {
    case LOW:
        // Port B handles D8 to D13
        PORTB &= ~(1UL << 2); // Set trigger pin D10 LOW
        break;
    case HIGH:
        PORTB |= 1UL << 2; // Set trigger pin D10 HIGH
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

float microsToCM(long microseconds)
{
    return (float)(microseconds / 2) / 29;
}