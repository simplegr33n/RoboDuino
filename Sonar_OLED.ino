#include <stdint.h>
#include <stdbool.h>
#include <TimerOne.h>

///////////
// For OLED
///////////
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
static const unsigned char PROGMEM logo_bmp[] =
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

// durations arrays for oled graphing
float D3_durations[10];
float D4_durations[10];
float D5_durations[10];
int durationsPointer = 0;

int dataUpdate = 0;

//////////////
// For Sensors
//////////////
volatile uint8_t portDstate, portDpast, changedBits;
volatile bool interruptCalled = false;

volatile unsigned long triggerTime;

const int numberOfSensors = 3;
unsigned long responseDurations[numberOfSensors];
unsigned long responseStarts[numberOfSensors] = {0, 0, 0};
int responseCount = 0; // reset to zero and trigger sensors when responseCount = numberOfSensors
volatile int pingCount = 0;

ISR(PCINT2_vect)
{
  portDpast = portDstate;
  portDstate = PIND; // get state of Port D with PIND
  changedBits = portDpast ^ portDstate;
  interruptCalled = true;
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

  /////////////
  // OLED Stuff
  /////////////
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  {                                               // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  //    drawGraph(); // Draw a small bitmap image

  /////////////////////////
  // Ping Interrupts Setup
  ////////////////////////
  cli();
  DDRD = 0b00000100;      // set all bits in Port B Data Direction Register to input, except PCINT18 (our trigger)
  setTriggerPinsTo(HIGH); // ensure trigger pins at high before enabling interrupts
  portDstate = PIND;
  PCICR |= (1 << PCIE2);                                      // Pin Change Interrupt Control Register enabling Port B
  PCMSK2 |= (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); // Enable mask on PCINT19 to trigger interupt on state change

  Timer1.initialize(100000);              // 500000 = half a second
  Timer1.attachInterrupt(triggerSensors); // triggerSensors to run every readingInterval
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
      if ((pingCount == 1) && (changedBits & 0b00001000))
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
      if ((pingCount == 2) && (changedBits & 0b00010000))
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
      if ((pingCount == 3) && (changedBits & 0b00100000))
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

    // Do stuff
    Serial.print(microsToCM(responseDurations[0])); // left
    Serial.print("cm|");
    Serial.print(microsToCM(responseDurations[1])); // center
    Serial.print("cm|");
    Serial.print(microsToCM(responseDurations[2])); // right
    Serial.println("cm");

    D3_durations[durationsPointer] = microsToCM(responseDurations[0]);
    D4_durations[durationsPointer] = microsToCM(responseDurations[1]);
    D5_durations[durationsPointer] = microsToCM(responseDurations[2]);

    sei();

    durationsPointer++;
    if (durationsPointer > 9)
    {
      durationsPointer = 0;

      drawGraph();
    }
  }
}

float microsToCM(long microseconds)
{
  return (float)(microseconds / 2) / 29;
}

/////////////
// drawGraph
////////////
void drawGraph(void)
{
  display.clearDisplay();

  // Top bar
  display.fillRect(0, 16, 128, 64, SSD1306_INVERSE);

  // Toby face
  display.drawBitmap(
      (display.width() - LOGO_WIDTH) / 2,
      0,
      logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);

  // Invert
  display.invertDisplay(true);

  // Graphs..
  // MIDDLE (40-(64)-88)
  drawCenterLines();

  // LEFT (0-32)
  drawLeftLines();

  // RIGHT (96-128)
  drawRightLines();

  display.display();
}

void drawCenterLines(void)
{
  int currentLine = 16;
  for (unsigned int a = 0; a < sizeof(D4_durations) / sizeof(D4_durations[0]); a++)
  {
    int nearness;
    if (D4_durations[a] > 100)
    {
      nearness = 0;
    }
    else if (D4_durations[a] > 60)
    {
      nearness = 0.25;
    }
    else if (D4_durations[a] > 30)
    {
      nearness = 0.5;
    }
    else
    {
      nearness = 1;
    }

    display.drawLine(65 - (24 * nearness), currentLine, 64 + (24 * nearness), currentLine, SSD1306_BLACK);
    currentLine++;
  }
}

void drawLeftLines(void)
{
  int currentLine = 16;
  for (unsigned int a = 0; a < sizeof(D3_durations) / sizeof(D3_durations[0]); a++)
  {
    int nearness;
    if (D3_durations[a] > 100)
    {
      nearness = 0;
    }
    else if (D3_durations[a] > 60)
    {
      nearness = 0.25;
    }
    else if (D3_durations[a] > 30)
    {
      nearness = 0.5;
    }
    else
    {
      nearness = 1;
    }

    display.drawLine(0, currentLine, (32 * nearness), currentLine, SSD1306_BLACK);
    currentLine++;
  }
}

void drawRightLines(void)
{
  int currentLine = 16;
  for (unsigned int a = 0; a < sizeof(D3_durations) / sizeof(D3_durations[0]); a++)
  {
    int nearness;
    if (D3_durations[a] > 100)
    {
      nearness = 0;
    }
    else if (D3_durations[a] > 60)
    {
      nearness = 0.25;
    }
    else if (D3_durations[a] > 30)
    {
      nearness = 0.5;
    }
    else
    {
      nearness = 1;
    }

    display.drawLine(128 - (32 * nearness), currentLine, 127, currentLine, SSD1306_BLACK);
    currentLine++;
  }
}
