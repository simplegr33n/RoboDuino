#include <TimerOne.h>

#define ULTRASONIC_INTERVAL 80000 // needs some tuning (500000 = half a second, 30000 perhaps minimum)

unsigned long ultrasonicResponseStarts[ultrasonicSensorQuantity];
volatile int ultrasonicPingCount = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initUltrasonicInterrupts()
{
    // Initialize Ultrasonic Ping Interrupts
    cli();
    DDRB = 0b00010000;      // set all bits in Port B Data Direction Register to input, except D10
    setTriggerPinsTo(HIGH); // ensure trigger pins at high before enabling interrupts
    DDRK = 0b00000000;      // Set all bits in Port D Data Direction Register to input
    portKstate = PINK;

    PCICR |= (1 << PCIE2);                                      // Pin Change Interrupt Control Register enabling Port K
    PCMSK2 |= (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23); // Enable mask on PCINT21-23 to trigger interrupt on state change

    Timer1.initialize(ULTRASONIC_INTERVAL);
    Timer1.attachInterrupt(triggerSensors); // triggerSensors every defined reading interval
    sei();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routine and related functions                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(PCINT2_vect)
{
    portKpast = portKstate;
    portKstate = PINK; // get state of Port K with PINK
    changedBitsK = portKpast ^ portKstate;
    ultrasonicInterruptCalled = true;
}

void resolveUltrasonicInterrupt()
{

    ultrasonicInterruptCalled = false;

    if (ultrasonicResponseCount != ultrasonicSensorQuantity)
    {

        // Get durations for each sensor response (one sensor per ping for now)
        if ((ultrasonicPingCount == 1) && (changedBitsK & 0b00100000))
        {
            if (ultrasonicResponseStarts[0] != 0)
            {
                Serial.println("CENTER");
                ultrasonicResponseDurations[0] = (micros() - ultrasonicResponseStarts[0]);
                ultrasonicResponseCount++;
            }
            else
            {
                ultrasonicResponseStarts[0] = micros();
            }
        }
        if ((ultrasonicPingCount == 2) && (changedBitsK & 0b01000000))
        {
            if (ultrasonicResponseStarts[1] != 0)
            {
                Serial.println("LEFT");
                ultrasonicResponseDurations[1] = (micros() - ultrasonicResponseStarts[1]);
                ultrasonicResponseCount++;
            }
            else
            {
                ultrasonicResponseStarts[1] = micros();
            }
        }
        if ((ultrasonicPingCount == 3) && (changedBitsK & 0b10000000))
        {
            if (ultrasonicResponseStarts[2] != 0)
            {
                Serial.println("RIGHT");
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
    // Refresh Interrupt counts
    ultrasonicResponseCount = 0;
    ultrasonicPingCount = 0;

    // Update history entries
    D2_history[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[0]);
    D3_history[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[1]);
    D4_history[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[2]);

    // Update pointer
    ultrasonicHistoryPointer++;
    if (ultrasonicHistoryPointer > 7)
    {
        ultrasonicHistoryPointer = 0;
    }
    sei();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trigger Sensors functions                                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    setTriggerPinsTo(LOW); // Set the trigger pins to LOW, falling edge triggers the sensor
    delayMicroseconds(10);
    setTriggerPinsTo(HIGH); // Then reset and leave HIGH, ready for next trigger
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utility functions                                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
float microsToCentimeters(long microseconds)
{
    return (float)microseconds / (29 * 2); // :: (duration [μs]) / (29 [cm/μs] * 2 [return distance])
}
