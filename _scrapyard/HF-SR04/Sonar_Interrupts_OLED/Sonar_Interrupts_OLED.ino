#include <stdint.h>
#include <stdbool.h>
#include <TimerOne.h>

volatile uint8_t portDstate, portDpast, changedBitsD;
volatile bool ultrasonicInterruptCalled = false;

volatile unsigned long triggerTime;

const int ultrasonicSensorQuantity = 3;
unsigned long ultrasonicResponseDurations[ultrasonicSensorQuantity];
unsigned long ultrasonicResponseStarts[ultrasonicSensorQuantity] = {0, 0, 0};
int ultrasonicResponseCount = 0; // reset to zero and trigger sensors when responseCount = numberOfSensors
volatile int ultrasonicPingCount = 0;

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
int D3_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D4_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int D5_history[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// Pointer for referencing position in history arrays
int ultrasonicHistoryPointer = 0;

ISR(PCINT2_vect)
{
  portDpast = portDstate;
  portDstate = PIND; // get state of Port D with PIND
  changedBitsD = portDpast ^ portDstate;
  ultrasonicInterruptCalled = true;
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
  Serial.begin(9600);

  // Get display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Draw unchanging header on display
  displayUltrasonicHeader();

  cli();
  DDRD = 0b00000100;      // set all bits in Port B Data Direction Register to input, except PCINT18 (our trigger)
  setTriggerPinsTo(HIGH); // ensure trigger pins at high before enabling interrupts
  portDstate = PIND;

  PCICR |= (1 << PCIE2);                                      // Pin Change Interrupt Control Register enabling Port B
  PCMSK2 |= (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); // Enable mask on PCINT19 to trigger interupt on state change

  Timer1.initialize(30000);               // 500000 = half a second, 30000 perhaps minimum
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
      if ((ultrasonicPingCount == 1) && (changedBitsD & 0b00001000))
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
      if ((ultrasonicPingCount == 2) && (changedBitsD & 0b00010000))
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
      if ((ultrasonicPingCount == 3) && (changedBitsD & 0b00100000))
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

    D3_history[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[0]);
    D4_history[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[1]);
    D5_history[ultrasonicHistoryPointer] = microsToCentimeters(ultrasonicResponseDurations[2]);

    // Update pointer
    ultrasonicHistoryPointer++;
    if (ultrasonicHistoryPointer > 7)
    {
      ultrasonicHistoryPointer = 0;
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
    updateUltrasonicGraph();
  }
}

float microsToCentimeters(long microseconds)
{
  return (float)(microseconds / 2) / 29;
}

/////////////////////////////////
// OLED Display functions
////////////////////////////////
void displayUltrasonicHeader(void)
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

void updateUltrasonicGraph(void)
{
  // Clear graph area for update
  display.fillRect(0, 16, 128, 48, SSD1306_WHITE);

  // Graph lines
  drawUltrasonicGraphLines();

  display.display();
}

void drawUltrasonicGraphLines(void)
{
  int currentLine = 0;
  for (unsigned int a = 0; a < 8; a++)
  {
    // determine actual currentIndex by reference to historyPointer, overflowing on 8
    int currentIndex = ultrasonicHistoryPointer + a;
    if (currentIndex >= 8)
    {
      currentIndex = 0 + (currentIndex - 8);
    }

    // Get left/right/center width ratios
    float leftWidth = (float)D3_history[currentIndex] / 50;
    if (leftWidth > 1)
    {
      leftWidth = 1;
    }
    float centerWidth = (float)D4_history[currentIndex] / 50;
    if (centerWidth > 1)
    {
      centerWidth = 1;
    }
    float rightWidth = (float)D5_history[currentIndex] / 50;
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