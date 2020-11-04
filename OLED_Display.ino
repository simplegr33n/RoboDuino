/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

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

int D3_history[8] = {0, 10, 33, 80, 4, 66, 63, 41};
int D4_history[8] = {0, 20, 30, 40, 50, 60, 80, 100};
int D5_history[8] = {15, 2, 150, 4, 25, 5, 73, 33};

int historyPointer = 0;

void setup()
{
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
}

void loop()
{
  // Update graphs
  drawGraph();
  delay(1000);

  // Update pointer
  historyPointer++;
  if (historyPointer > 7)
  {
    historyPointer = 0;
  }
}

void drawGraph(void)
{
  display.clearDisplay();

  // Top bar
  display.fillRect(0, 16, 128, 64, SSD1306_INVERSE);
  // display.drawRect(0, 0, 128, 16, SSD1306_INVERSE);

  // Toby face
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

  // Graphs..
  // MIDDLE (40-88)
  // LEFT (0-32)
  // RIGHT (96-128)
  drawGraphLines();

  display.display();
}

void drawGraphLines(void)
{
  int currentLine = 0;
  for (unsigned int a = 0; a < 8; a++)
  {
    // determine actual currentIndex by reference to historyPointer
    int currentIndex = historyPointer + a;
    if (currentIndex >= 8)
    {
      currentIndex = 0 + (currentIndex - 8);
    }

    // Get left/right/center width ratios
    float leftWidth = (float)D3_history[currentIndex] / 100;
    if (leftWidth > 1)
    {
      leftWidth = 1;
    }
    float centerWidth = (float)D4_history[currentIndex] / 100;
    if (centerWidth > 1)
    {
      centerWidth = 1;
    }
    float rightWidth = (float)D5_history[currentIndex] / 100;
    if (rightWidth > 1)
    {
      rightWidth = 1;
    }

    // draw L/C/R lines
    // display.drawLine(0, 16 + (currentLine * 6), (32 * leftWidth), 16 + (currentLine * 6), SSD1306_BLACK);
    //display.drawLine((65 - (int)(24 * centerWidth)), 16 + (currentLine * 6), (64 + (int)(24 * centerWidth)), 16 + (currentLine * 6), SSD1306_BLACK);
    //display.drawLine(128 - (32 * rightWidth), 16 + (currentLine * 6), 127, 16 + (currentLine * 6), SSD1306_BLACK);

    // (optional) draw L/C/R rects instead
    display.fillRect(0, 16 + (currentLine * 6), (32 * leftWidth), 6, SSD1306_BLACK);
    display.fillRect((65 - (int)(24 * centerWidth)), 16 + (currentLine * 6), ((int)(48 * centerWidth)), 6, SSD1306_BLACK);
    display.fillRect(128 - (32 * rightWidth), 16 + (currentLine * 6), (32 * rightWidth), 6, SSD1306_BLACK);

    currentLine++;
  }
}