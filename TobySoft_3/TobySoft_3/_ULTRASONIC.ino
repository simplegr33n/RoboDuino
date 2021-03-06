// Interrupt variables
volatile uint8_t portKstate, portKpast, changedBitsK;
volatile unsigned long ultrasonicResponseStarts[ultrasonicSensorQuantity];
volatile unsigned long ultrasonicResponseEnds[ultrasonicSensorQuantity];
volatile int ultrasonicResponseCount = 0; // trigger history update when ultrasonicResponseCount == ultrasonicSensorQuantity

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

    cli();

    ultrasonicInterruptCalled = false;

    if (ultrasonicResponseCount != ultrasonicSensorQuantity)
    {

        // Get durations for each sensor response (Sequentially based on ultrasonicResponseCount)
        if (ultrasonicResponseCount == 0 && (changedBitsK & 0b00100000) && (ultrasonicResponseEnds[0] == 0))
        {
            if (ultrasonicResponseStarts[0] != 0)
            {
                ultrasonicResponseEnds[0] = micros();
                ultrasonicResponseCount++;
            }
            else
            {
                ultrasonicResponseStarts[0] = micros();
            }
        }
        if (ultrasonicResponseCount == 1 && (changedBitsK & 0b01000000) && (ultrasonicResponseEnds[1] == 0))
        {
            if (ultrasonicResponseStarts[1] != 0)
            {
                ultrasonicResponseEnds[1] = micros();
                ultrasonicResponseCount++;
            }
            else
            {
                ultrasonicResponseStarts[1] = micros();
            }
        }
        if (ultrasonicResponseCount == 2 && (changedBitsK & 0b10000000) && (ultrasonicResponseEnds[2] == 0))
        {
            if (ultrasonicResponseStarts[2] != 0)
            {
                ultrasonicResponseEnds[2] = micros();
                ultrasonicResponseCount++;
            }
            else
            {
                ultrasonicResponseStarts[2] = micros();
            }
        }
    }

    sei();
}

void updateUltrasonicHistory()
{
    cli();
    // Refresh Interrupt counts
    ultrasonicResponseCount = 0;

    // update durations
    ultrasonicResponseDurations[0] = ultrasonicResponseEnds[0] - ultrasonicResponseStarts[0];
    ultrasonicResponseDurations[1] = ultrasonicResponseEnds[1] - ultrasonicResponseStarts[1];
    ultrasonicResponseDurations[2] = ultrasonicResponseEnds[2] - ultrasonicResponseStarts[2];

    // Update history entries
    A13_History[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[0]);
    A14_History[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[1]);
    A15_History[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[2]);

    // Update pointer
    ultrasonicHistoryPointer++;
    if (ultrasonicHistoryPointer > 7)
    {
        ultrasonicHistoryPointer = 0;
    }

    // Refresh response starts (todo: loop)
    ultrasonicResponseStarts[0] = 0;
    ultrasonicResponseStarts[1] = 0;
    ultrasonicResponseStarts[2] = 0;
    // Refresh response ends
    ultrasonicResponseEnds[0] = 0;
    ultrasonicResponseEnds[1] = 0;
    ultrasonicResponseEnds[2] = 0;

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
        updateUltrasonicHistory();
        if (AUTOPILOT_ON) // TODO: remove... for now just so i can enjoy sleep screen, eventually need more advanced display system.
        {
            updateUltrasonicGraph();
        }
    }

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
