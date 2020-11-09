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

#define SCREEN_WIDTH 128 // OLED pixel width
#define SCREEN_HEIGHT 64 // OLED pixel height

#define OLED_RESET 4                                                      // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initOLED()
{
    // Get display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    {                                               // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    // Draw unchanging header on display
    displayUltrasonicHeader();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// OLED Display functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
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
