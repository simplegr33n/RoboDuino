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
#define READING_INTERVAL 40000

///////////////////////
// OLED / ADAFRUIT Jazz
///////////////////////
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM lena_bmp[] =
    {B00000000, B00000000,
     B01110000, B00000110,
     B01101100, B00000111,
     B01000110, B00011001,
     B01010111, B11100101,
     B01010000, B00000101,
     B01101110, B11100011,
     B00110101, B01010001,
     B01110011, B00110011,
     B11100000, B00000011,
     B11010001, B11000110,
     B01010100, B10000011,
     B00101011, B11101110,
     B10100100, B00010010,
     B10110001, B11100110,
     B11011000, B00011100};

// Historical distance readings
int D2_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D3_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D4_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// Pointer for referencing position in history arrays
int historyPointer = 0;

///////////////////////
// For Driving
///////////////////////
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6
#define carSpeed 200
int middleDistance, leftDistance, rightDistance;
unsigned long lastPilotDecision = 0; // micros() timestamp of last autoPilotDecision

ISR(PCINT2_vect)
{
  portDpast = portDstate;
  portDstate = PIND; // Get state of Port D with PIND
  changedBits = portDpast ^ portDstate;
  interruptCalled = true;
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
  Serial.begin(9600);

  // Get display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  {                                               // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Draw unchanging header on display
  drawHeader();

  // Set up Motor Output Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();

  // Initialize Ping Interrupts
  cli();
  DDRD = 0b00000000;      // Set all bits in Port D Data Direction Register to input
  DDRB = 0b00000100;      // Set our trigger pin (D10) to output
  setTriggerPinsTo(HIGH); // Ensure trigger pins at high before enabling interrupts
  portDstate = PIND;

  PCICR |= (1 << PCIE2);                                      // Pin Change Interrupt Control Register enabling Port D
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20); // Enable mask on PCINT18-20 to trigger interrupt on state change

  Timer1.initialize(READING_INTERVAL);    // (500000 = half a second, 30000 perhaps minimum)
  Timer1.attachInterrupt(triggerSensors); // TriggerSensors to run every readingInterval
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
      if ((pingCount == 1) && (changedBits & 0b00000100))
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
      if ((pingCount == 2) && (changedBits & 0b00001000))
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
      if ((pingCount == 3) && (changedBits & 0b00010000))
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

    D2_history[historyPointer] = microsToCM(responseDurations[0]);
    D3_history[historyPointer] = microsToCM(responseDurations[1]);
    D4_history[historyPointer] = microsToCM(responseDurations[2]);

    leftDistance = D2_history[historyPointer]; // this sillyness only necessary because of graphing..
    middleDistance = D3_history[historyPointer];
    rightDistance = D4_history[historyPointer];

    // Update pointer
    historyPointer++;
    if (historyPointer > 7)
    {
      historyPointer = 0;
    }
    sei();

    // DEBUG
    //    Serial.print(microsToCM(responseDurations[0])); // left
    //    Serial.print("cm|");
    //    Serial.print(microsToCM(responseDurations[1])); // center
    //    Serial.print("cm|");
    //    Serial.print(microsToCM(responseDurations[2])); // right
    //    Serial.println("cm");

    // Update OLED Graph
    drawGraph();
  }

  // Drive car code
  if ((lastPilotDecision == 0) || (micros() - lastPilotDecision > 300000))
  {
    lastPilotDecision = micros();
    autoPilot();
  }
}

float microsToCM(long microseconds)
{
  return (float)(microseconds / 2) / 29;
}

/////////////////////////////////
// OLED Display functions
////////////////////////////////
void drawHeader(void)
{
  display.clearDisplay();

  // Top bar
  display.fillRect(0, 16, 128, 48, SSD1306_INVERSE);

  // Draw Toby face bmp
  display.drawBitmap(
      (display.width() - LOGO_WIDTH) / 2,
      0,
      lena_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);

  // L / R text
  display.setTextSize(2);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font
  display.setCursor(24, 0);            // Start at top-left corner
  display.write('L');
  display.setCursor(96, 0); // Start at top-left corner
  display.write('R');

  // Invert
  display.invertDisplay(true);

  display.display();
}

void drawGraph(void)
{
  // Clear graph area for update
  display.fillRect(0, 16, 128, 48, SSD1306_WHITE);

  // Graph lines
  drawGraphLines();

  display.display();
}

void drawGraphLines(void)
{
  int currentLine = 0;
  for (unsigned int a = 0; a < 8; a++)
  {
    // determine actual currentIndex by reference to historyPointer, overflowing on 8
    int currentIndex = historyPointer + a;
    if (currentIndex >= 8)
    {
      currentIndex = 0 + (currentIndex - 8);
    }

    // Get left/right/center width ratios
    float leftWidth = (float)D2_history[currentIndex] / 50;
    if (leftWidth > 1)
    {
      leftWidth = 1;
    }
    float centerWidth = (float)D3_history[currentIndex] / 50;
    if (centerWidth > 1)
    {
      centerWidth = 1;
    }
    float rightWidth = (float)D4_history[currentIndex] / 50;
    if (rightWidth > 1)
    {
      rightWidth = 1;
    }

    // draw L/C/R lines (with rects for thickness)
    display.fillRect(0, 16 + (currentLine * 6), (34 - (34 * leftWidth)), 6, SSD1306_BLACK);                                      // LEFT (0-34)
    display.fillRect((64 - (28 - (28 * centerWidth))), 16 + (currentLine * 6), (28 - (28 * centerWidth)) * 2, 6, SSD1306_BLACK); // CENTER (36-(64)-92)
    display.fillRect(128 - (34 - (34 * rightWidth)), 16 + (currentLine * 6), (34 - (34 * rightWidth)), 6, SSD1306_BLACK);        // RIGHT (94-128)

    currentLine++;
  }
}

/////////////////////////////////
// Motor Driving Functions
////////////////////////////////
void autoPilot()
{
  int leftDistance = microsToCM(responseDurations[0]);
  int middleDistance = microsToCM(responseDurations[1]);
  int rightDistance = microsToCM(responseDurations[2]);

  if ((middleDistance < 20) || (leftDistance < 10) || (rightDistance < 10))
  {
    stop();

    if ((rightDistance < 10) && (leftDistance < 10))
    {
      back();
    }
    else if (rightDistance > leftDistance)
    {
      right();
    }
    else if (rightDistance < leftDistance)
    {
      left();
    }
  }
  else
  {
    forward();
  }
}

void forward()
{
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back()
{
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left()
{
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Left");
}

void right()
{
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop()
{
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}
