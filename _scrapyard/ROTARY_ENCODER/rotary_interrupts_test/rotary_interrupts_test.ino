#define ENCODER_CLK_Pin A13 // PCINT21
#define ENCODER_DT_Pin A14  // PCINT22
#define ENCODER_SW_Pin A15

volatile int encoderCounter = 0;
volatile uint8_t prevEncoderPhase;
volatile uint8_t encoderPhase;

int encoderSwitchValue;

void setup()
{
    Serial.begin(115200);

    // initialize encoder switch
    pinMode(ENCODER_SW_Pin, INPUT_PULLUP);

    // Initialize Encoder innterupts
    cli();
    DDRK = 0b00000000; // Set all bits in Port D Data Direction Register to input
    encoderPhase = PINK;

    PCICR |= (1 << PCIE2);                     // Pin Change Interrupt Control Register enabling Port K
    PCMSK2 |= (1 << PCINT21) | (1 << PCINT22); // Enable mask on PCINT21-22 to trigger interrupt on state change
    sei();
}

// Handle pin change interrupt request 2.
ISR(PCINT2_vect)
{
    prevEncoderPhase = encoderPhase;
    encoderPhase = PINK; // get state of Port K with PINK (PIND on nano)

    if (prevEncoderPhase == 192 && encoderPhase == 224)
    {
        encoderCounter += 1;
    }
    else if (prevEncoderPhase == 160 && encoderPhase == 224)
    {
        encoderCounter -= 1;
    }

    // DEBUG
    Serial.print("PREV: ");
    Serial.print(prevEncoderPhase);
    Serial.print(", CUR: ");
    Serial.println(encoderPhase);
    Serial.print("COUNTER: ");
    Serial.print(encoderCounter);
    Serial.print("sw:");
    Serial.print(digitalRead(ENCODER_SW_Pin));
}

void loop()
{
    // doing nothing
}
