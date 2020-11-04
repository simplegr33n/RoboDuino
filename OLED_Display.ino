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

  testdrawbitmap(); // Draw a small bitmap image

  delay(1000);
}

void loop()
{
  // Update graphs
}

void testdrawbitmap(void)
{
  display.clearDisplay();

  // Top bar
  display.fillRect(0, 16, 128, 64, SSD1306_INVERSE);
  // display.drawRect(0, 0, 128, 16, SSD1306_INVERSE);

  // Toby face
  display.drawBitmap(
      (display.width() - LOGO_WIDTH) / 2,
      0,
      logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);

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
  display.drawLine(48, 16, 80, 16, SSD1306_BLACK);
  display.drawLine(60, 17, 68, 17, SSD1306_BLACK);
  display.drawLine(54, 18, 74, 18, SSD1306_BLACK);
  display.drawLine(40, 19, 88, 19, SSD1306_BLACK);
  display.drawLine(60, 20, 68, 20, SSD1306_BLACK);
  display.drawLine(54, 21, 74, 21, SSD1306_BLACK);
  display.drawLine(40, 22, 88, 22, SSD1306_BLACK);

  // LEFT (0-32)
  display.drawLine(0, 16, 32, 16, SSD1306_BLACK);
  display.drawLine(0, 17, 31, 17, SSD1306_BLACK);
  display.drawLine(0, 18, 16, 18, SSD1306_BLACK);
  display.drawLine(0, 19, 6, 19, SSD1306_BLACK);
  display.drawLine(0, 20, 9, 20, SSD1306_BLACK);
  display.drawLine(0, 21, 32, 21, SSD1306_BLACK);
  display.drawLine(0, 22, 32, 22, SSD1306_BLACK);

  // RIGHT (96-128)
  display.drawLine(114, 16, 128, 16, SSD1306_BLACK);
  display.drawLine(116, 17, 128, 17, SSD1306_BLACK);
  display.drawLine(99, 18, 128, 18, SSD1306_BLACK);
  display.drawLine(105, 19, 128, 19, SSD1306_BLACK);
  display.drawLine(96, 20, 128, 20, SSD1306_BLACK);
  display.drawLine(113, 21, 128, 21, SSD1306_BLACK);
  display.drawLine(0, 22, 32, 22, SSD1306_BLACK);

  display.display();
  delay(1000);
}