#include <TimerOne.h>

#define ULTRASONIC_INTERVAL 80000 // needs some tuning

unsigned long ultrasonicResponseStarts[ultrasonicSensorQuantity];
volatile int ultrasonicPingCount = 0;

///////////////////////
//                   //
//  END GLOBAL VARS  //
//                   //
///////////////////////

void initUltrasonicInterrupts()
{
    // Initialize Ultrasonic Ping Interrupts
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

/////////////////////////////////
// Interrupt Service Routine
////////////////////////////////
ISR(PCINT2_vect)
{
    portDpast = portDstate;                    // Update past Port D state
    portDstate = PIND;                         // Get current Port D state
    portDchangedBits = portDpast ^ portDstate; // Check for changed bits on Port D to determine precise echoing pin
    ultrasonicInterruptCalled = true;
}

void resolveUltrasonicInterrupt()
{

    ultrasonicInterruptCalled = false;

    if (ultrasonicResponseCount != ultrasonicSensorQuantity)
    {

        // Get durations for each sensor response (one sensor per ping for now)
        if ((ultrasonicPingCount == 1) && (portDchangedBits & 0b00000100))
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
        if ((ultrasonicPingCount == 2) && (portDchangedBits & 0b00001000))
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
        if ((ultrasonicPingCount == 3) && (portDchangedBits & 0b00010000))
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

/////////////////////////////////
// Trigger Functions
////////////////////////////////
void setTriggerPinsTo(uint8_t signalState)
{
    switch (signalState)
    {
    case LOW:
        // Port B handles D8 to D13 - setting bit 2 sets trigger pin D10 HIGH or LOW
        PORTB &= ~(1UL << 2);
        break;
    case HIGH:
        PORTB |= 1UL << 2;
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

/////////////////////////////////
// Utility Functions
////////////////////////////////
float microsToCentimeters(long microseconds)
{
    return (float)microseconds / (29 * 2); // :: (duration [μs]) / (29 [cm/μs] * 2 [return distance])
}