#define IRMUX_DATA_PIN 2
#define IRMUX_CLOCK_PIN 4
#define IRMUX_LATCH_PIN 3
#define IRMUX_ANALOG_PIN A0

byte data = 0;

int irRead0;

void setup()
{
  Serial.begin(115200);

  // Set the three SPI pins to be outputs:
  pinMode(IRMUX_DATA_PIN, OUTPUT);
  pinMode(IRMUX_CLOCK_PIN, OUTPUT);
  pinMode(IRMUX_LATCH_PIN, OUTPUT);
}

void loop()
{
  oneOnAtATime(); // Scroll down the line
}

void shiftWrite(int desiredPin, boolean desiredState)
{
  // First we'll alter the global variable "data", changing the
  // desired bit to 1 or 0:
  bitWrite(data, desiredPin, desiredState);

  // The shiftOut() function manipulates the data and clock
  // pins to move the data into the shift register:
  shiftOut(IRMUX_DATA_PIN, IRMUX_CLOCK_PIN, MSBFIRST, data);

  // Toggle IRMUX_LATCH_PIN to signal the shift register to "latch"
  // the data to the outputs. (Rising edge activation)
  digitalWrite(IRMUX_LATCH_PIN, HIGH);
  digitalWrite(IRMUX_LATCH_PIN, LOW);
}

// step through the emitter LEDs, lighting one at at time.

void oneOnAtATime()
{
  int index;
  int delayTime = 500; // Time (milliseconds) to pause between LEDs

  // step through the ir emitter LEDs, from 0 to 7

  for (index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH); // turn LED on
    delay(delayTime);        // pause to slow down the sequence
    shiftWrite(index, LOW);  // turn LED off

    // check prox
    irRead0 = analogRead(A0);
    //irRead1 = analogRead(A1)

    Serial.print(index);
    Serial.print(" read: ");
    Serial.println(irRead0);
  }
}
