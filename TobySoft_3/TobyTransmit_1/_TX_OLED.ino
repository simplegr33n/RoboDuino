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

int OLED_RESET = 4;                                                       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

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

    displayInitSequence(); // play OLED init sequence
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// OLED Display functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void displayInitSequence(void)
{
    display.clearDisplay();
    // Draw TobySoft 3.0 bootscreen
    display.drawBitmap(
        0,
        0,
        TS3_BMP, 128, 64, 1);

    display.display();
    delay(1000); // show for 1 seconds
}

void updateHomeScreen()
{
    display.clearDisplay();

    displaySendHeader(); // show send data in header
    displayRxData();     // show rx data in the body
}

void displaySendHeader(void)
{
    // Top bar
    display.fillRect(0, 0, 128, 16, SSD1306_WHITE);

    // L / R text
    display.setTextSize(1.5); // 2:1 pixel scale
    display.setTextColor(SSD1306_BLACK);

    display.setCursor(8, 1); // Start at top-left corner
    display.println('x');
    display.setCursor(22, 1);
    display.println(xAngleValue);
    //
    display.setCursor(54, 1);
    display.println('y');
    display.setCursor(68, 1);
    display.println(yAngleValue);
    //
    display.setCursor(98, 1);
    display.println('s');
    display.setCursor(110, 1);
    display.println(SW_PinRead);

    display.display();
}

void displayRxData(void)
{
    if (ackData[0] != -1)
    {
        // L / R text
        display.setTextSize(2); // 3:1 pixel scale
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(6, 40);
        display.println(ackData[1]);
        //
        display.setCursor(56, 40);
        display.println(ackData[0]);
        //
        display.setCursor(100, 40);
        display.println(ackData[2]);

        display.display();
    }
    else
    {
        // L / R text
        display.setTextSize(3.5); // 3:1 pixel scale
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(6, 30);
        display.println("N/C!");

        display.display();
    }
}
