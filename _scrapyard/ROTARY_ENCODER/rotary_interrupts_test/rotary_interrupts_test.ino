#define ENCODER_CLK_Pin A13 // PCINT21
#define ENCODER_DT_Pin A14  // PCINT22
#define ENCODER_SW_Pin A15

volatile int encoderCounter = 0;
volatile uint8_t PREV_ENCODER_PHASE;
volatile uint8_t ENCODER_PHASE;

int encoderSwitchValue;

void setup()
{
    Serial.begin(115200);

    // initialize encoder switch
    pinMode(ENCODER_SW_Pin, INPUT);

    // Initialize Encoder innterupts
    cli();
    DDRK = 0b00000000; // Set all bits in Port D Data Direction Register to input
    ENCODER_PHASE = PINK;

    PCICR |= (1 << PCIE2);                     // Pin Change Interrupt Control Register enabling Port K
    PCMSK2 |= (1 << PCINT21) | (1 << PCINT22); // Enable mask on PCINT21-22 to trigger interrupt on state change
    sei();
}

// Handle pin change interrupt request 2.
ISR(PCINT2_vect)
{
    PREV_ENCODER_PHASE = ENCODER_PHASE;
    ENCODER_PHASE = PINK; // get state of Port K with PINK (PIND on nano)

    if (PREV_ENCODER_PHASE == 192 && ENCODER_PHASE == 224)
    {
        encoderCounter += 1;
    }
    else if (PREV_ENCODER_PHASE == 160 && ENCODER_PHASE == 224)
    {
        encoderCounter -= 1;
    }

    // DEBUG
    Serial.print("PREV: ");
    Serial.print(PREV_ENCODER_PHASE);
    Serial.print(", CUR: ");
    Serial.println(ENCODER_PHASE);
    Serial.print("COUNTER: ");
    Serial.print(encoderCounter);
    Serial.print("sw:");
    Serial.print(digitalRead(ENCODER_SW_Pin));
}

void loop()
{
    // doing nothing
}
