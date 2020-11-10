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
// Bitmaps                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned char PROGMEM tobysoft_2[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0xaa, 0xaa, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xaa, 0xaa, 0xab, 0xfd, 0x1f, 0xe3, 0xe7, 0xf3, 0xde, 0x7c, 0x3e, 0x7f, 0xbf, 0xc0, 0x00,
    0x00, 0x2a, 0xaa, 0xaa, 0x05, 0x1b, 0x66, 0x63, 0x19, 0x98, 0xcc, 0x66, 0x31, 0xb6, 0xcf, 0xf0,
    0x00, 0x0a, 0xaa, 0xab, 0xfd, 0x1b, 0x6c, 0x33, 0x18, 0xd8, 0xc6, 0xc3, 0x30, 0x36, 0xcf, 0xf0,
    0x00, 0x02, 0xaa, 0xaa, 0x2d, 0x03, 0x0c, 0x33, 0x18, 0xf0, 0xe0, 0xc3, 0x33, 0x06, 0x00, 0x30,
    0x00, 0x00, 0xaa, 0xab, 0xfd, 0x03, 0x0c, 0x33, 0x30, 0x70, 0x70, 0xc3, 0x33, 0x06, 0x0f, 0xf0,
    0x00, 0x00, 0x2a, 0xa8, 0x01, 0x03, 0x0c, 0x33, 0xf0, 0x60, 0x38, 0xc3, 0x3f, 0x06, 0x0f, 0xf0,
    0x00, 0x00, 0x0a, 0xaf, 0xff, 0x03, 0x0c, 0x33, 0x18, 0x60, 0x0c, 0xc3, 0x33, 0x06, 0x00, 0x30,
    0x00, 0x00, 0x02, 0xae, 0x07, 0x03, 0x0c, 0x33, 0x18, 0x60, 0x06, 0xc3, 0x30, 0x06, 0x0f, 0xf0,
    0x00, 0x00, 0x00, 0xae, 0xf7, 0x03, 0x0c, 0x33, 0x18, 0x60, 0xc6, 0xc3, 0x30, 0x06, 0x0f, 0xf0,
    0x00, 0x00, 0x00, 0x2e, 0xd7, 0x03, 0x06, 0x63, 0x18, 0x60, 0xc6, 0x66, 0x30, 0x06, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0e, 0xd6, 0x07, 0xc3, 0xe7, 0xf0, 0xf0, 0xfc, 0x3e, 0x78, 0x0f, 0x8f, 0xf0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0xf8, 0x7e, 0x1c, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0xf0, 0x7e, 0x1c, 0x70, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0xc0, 0x9e, 0x18, 0x70, 0x00, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x02, 0x03, 0xd8, 0x70, 0x00, 0x00, 0x80, 0x00, 0x7f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x0c, 0x00, 0xf8, 0x70, 0x00, 0x00, 0x80, 0x00, 0x7f, 0xff, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x18, 0x00, 0x78, 0x70, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xe2,
    0x18, 0x00, 0x7c, 0x70, 0x00, 0x60, 0x00, 0x00, 0x02, 0x1c, 0x20, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x18, 0x00, 0x3c, 0x70, 0x00, 0x40, 0x00, 0x00, 0x0e, 0x18, 0x70, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x18, 0x00, 0x1c, 0x70, 0x00, 0x84, 0x00, 0x00, 0x07, 0x38, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x88, 0x00, 0x1c, 0x60, 0x01, 0x8c, 0x01, 0x00, 0x07, 0x39, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x88, 0x00, 0x5c, 0x60, 0x01, 0x98, 0x00, 0x80, 0x03, 0xb7, 0x80, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x00, 0x00, 0x0c, 0xe0, 0x03, 0x98, 0x00, 0x00, 0x02, 0x72, 0x00, 0x41, 0xff, 0xff, 0xfc, 0x12,
    0x04, 0x00, 0x0c, 0xe0, 0x03, 0x18, 0x00, 0x00, 0x00, 0x70, 0x00, 0x41, 0xff, 0xff, 0xfc, 0x12,
    0x00, 0x00, 0x0c, 0xe0, 0x01, 0x18, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x41, 0xff, 0xff, 0xfc, 0x12,
    0x00, 0x00, 0x00, 0xe0, 0x00, 0x08, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x41, 0xc7, 0x8f, 0x1c, 0x12,
    0x00, 0x00, 0x00, 0xe0, 0x00, 0x04, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0xc6, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0x1e, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0x0e, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0x04, 0x03, 0x80, 0x00, 0x00, 0x80, 0x00, 0x01, 0x80, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 0x03, 0x80, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x20, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xdf, 0xf8, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0xc0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xf8, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x20, 0x00, 0x03, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0x00, 0xff, 0x03, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x07, 0x00, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0xff, 0xff, 0xc0, 0x40, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x3f, 0xe0, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x7f, 0xe0, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0xff, 0xff, 0xf8, 0x00, 0x80, 0x00, 0x00, 0x00, 0xc7, 0x70, 0xe0, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0xff, 0x24, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc6, 0x7f, 0xc0, 0x41, 0x83, 0x06, 0x0c, 0x12,
    0xfe, 0x04, 0x7f, 0x00, 0x0c, 0x00, 0x00, 0x01, 0x86, 0xff, 0xc0, 0x41, 0xc7, 0x8f, 0x1c, 0x12,
    0x7f, 0xff, 0xff, 0x00, 0x3e, 0x00, 0x00, 0x01, 0xfe, 0xe1, 0xc0, 0x41, 0xff, 0xff, 0xfc, 0x12,
    0x3e, 0x44, 0x7f, 0x80, 0x7f, 0x80, 0x00, 0x03, 0xfc, 0xff, 0x80, 0x41, 0xff, 0xff, 0xfc, 0x12,
    0x1c, 0x04, 0x7f, 0xc0, 0xff, 0xff, 0xc0, 0x01, 0x11, 0xff, 0x80, 0x41, 0xff, 0xff, 0xfc, 0x12,
    0x9e, 0x46, 0xff, 0xc1, 0xff, 0xff, 0xf0, 0x03, 0x1d, 0x83, 0x80, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x1f, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0xf8, 0x03, 0x39, 0x83, 0x00, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x8e, 0x04, 0x4f, 0xe1, 0xff, 0xff, 0xf8, 0x03, 0x73, 0xff, 0x00, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x0f, 0x22, 0x47, 0xe1, 0xff, 0xff, 0xf0, 0x04, 0x7b, 0xff, 0x00, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x07, 0xff, 0x4f, 0xe3, 0xff, 0xff, 0xf0, 0x1f, 0xf0, 0x88, 0x00, 0x40, 0x00, 0x00, 0x00, 0x12,
    0x1f, 0xec, 0xff, 0x01, 0xff, 0xff, 0xe0, 0x1f, 0x87, 0x9e, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xe2,
    0x1f, 0xe0, 0xfe, 0x01, 0xff, 0xff, 0xe0, 0x18, 0x0f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x0f, 0xe4, 0xc0, 0x00, 0x7f, 0xff, 0xc0, 0x00, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x07, 0xfe, 0x00, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0xc3, 0xff, 0xc3, 0xff, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0xe0, 0x7f, 0xff, 0xff, 0xc1, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0x78, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'lena_lay', 128x64px
const unsigned char lena_lay[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x3f,
    0xc0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xb8, 0x3f, 0xff, 0xff, 0xe3, 0x3f,
    0xc0, 0x5f, 0x80, 0x40, 0x20, 0x11, 0x39, 0x07, 0xff, 0xbf, 0x78, 0x1f, 0xff, 0xff, 0xc7, 0x1f,
    0xc0, 0x5f, 0x80, 0x40, 0x20, 0x11, 0x39, 0x07, 0xff, 0x7e, 0xf8, 0x0f, 0xff, 0xff, 0x0f, 0x1f,
    0xdf, 0xdf, 0x9f, 0xcf, 0xe7, 0xd1, 0x19, 0x27, 0xfe, 0xfe, 0x38, 0x07, 0xff, 0xfe, 0x1f, 0x1f,
    0xdf, 0xdf, 0x9f, 0xcf, 0xe7, 0xdb, 0x1d, 0x3f, 0xfe, 0x3f, 0xf9, 0xc3, 0xff, 0xfc, 0x3e, 0x1f,
    0xc0, 0x5f, 0x9f, 0xcf, 0xe7, 0xdb, 0x2d, 0x3f, 0xff, 0xe3, 0xf9, 0xe0, 0xff, 0x80, 0x3c, 0x3f,
    0xff, 0x5f, 0x83, 0xc1, 0xe0, 0x1b, 0x2d, 0x3f, 0xff, 0xfb, 0x18, 0xf0, 0x00, 0x40, 0x38, 0x3f,
    0xff, 0x5f, 0x9f, 0xcf, 0xe0, 0x1b, 0x25, 0x23, 0xfc, 0x77, 0xdc, 0xf0, 0x00, 0x00, 0xfc, 0x3f,
    0xff, 0x5f, 0x9f, 0xcf, 0xe0, 0x1b, 0x31, 0x23, 0xff, 0x6f, 0xbc, 0x78, 0x00, 0x00, 0x78, 0x7f,
    0xff, 0x5f, 0x9f, 0xcf, 0xe7, 0xfb, 0x31, 0x23, 0xfe, 0xe3, 0x7c, 0x78, 0x02, 0x40, 0x00, 0x7f,
    0x80, 0x40, 0x80, 0x40, 0x27, 0xf1, 0x39, 0x27, 0xfd, 0xff, 0x1e, 0x78, 0x02, 0x40, 0x01, 0x7f,
    0x80, 0x40, 0x80, 0x40, 0x27, 0xf1, 0x39, 0x07, 0x24, 0x7f, 0xfe, 0x70, 0x02, 0x40, 0x01, 0x7f,
    0x80, 0x40, 0x80, 0x40, 0x27, 0xf1, 0x39, 0x07, 0x27, 0xff, 0xfe, 0x00, 0x28, 0x14, 0x10, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x28, 0x14, 0x10, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0xc8, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x3c, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x00, 0x3f,
    0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x07, 0x03, 0x3f,
    0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x3f,
    0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3f,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3d,
    0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31,
    0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03,
    0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x7f,
    0xf8, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x3c, 0xfe, 0x00, 0x7f,
    0xe0, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x6c, 0xab, 0x00, 0x7f,
    0x80, 0x30, 0x18, 0x00, 0xe0, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x04, 0x44, 0x74, 0xfe, 0x00, 0xff,
    0x00, 0x01, 0xd8, 0x04, 0x30, 0x23, 0x80, 0x00, 0x00, 0x10, 0x00, 0x90, 0x3c, 0xaf, 0x00, 0x7f,
    0x00, 0x01, 0xf0, 0x00, 0x1c, 0x40, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x24, 0x5c, 0xfb, 0x00, 0x3f,
    0x01, 0xe0, 0x10, 0x00, 0x0f, 0x00, 0x3f, 0x00, 0x00, 0x04, 0x01, 0x68, 0x60, 0x0e, 0x00, 0x0f,
    0x00, 0x78, 0x04, 0x20, 0x07, 0x38, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x48, 0x1f, 0xf8, 0x00, 0x07,
    0x00, 0x00, 0x60, 0x10, 0x06, 0x7c, 0x03, 0x80, 0x00, 0x00, 0x82, 0x00, 0x07, 0xf0, 0x00, 0x13,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0xf8, 0x01, 0x80, 0x00, 0x00, 0x80, 0x40, 0x00, 0xe0, 0x00, 0x01,
    0x1f, 0xc0, 0x00, 0x89, 0x04, 0x78, 0x61, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01,
    0x0e, 0x3f, 0xc0, 0x01, 0x01, 0x38, 0x78, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x10,
    0x08, 0x0f, 0xe0, 0x00, 0x01, 0x88, 0xf8, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xe7, 0x88,
    0x00, 0x00, 0x04, 0x00, 0x01, 0xc0, 0xf8, 0x46, 0x00, 0x00, 0x20, 0x00, 0x00, 0x03, 0x37, 0xce,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x40, 0x78, 0x46, 0x00, 0x00, 0x20, 0x00, 0x00, 0x01, 0x73, 0xe3,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x42, 0x38, 0x06, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x8b, 0xf1,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x83, 0x18, 0x23, 0x80, 0x00, 0x48, 0x08, 0x58, 0x07, 0xe1, 0xf3,
    0x00, 0x00, 0x04, 0x08, 0x43, 0x07, 0x10, 0x03, 0x80, 0x04, 0x40, 0x00, 0x80, 0x04, 0xe1, 0xfb,
    0x00, 0x00, 0x10, 0x48, 0x60, 0x0f, 0x10, 0x03, 0x00, 0x00, 0x00, 0x01, 0x06, 0x41, 0xb1, 0xff,
    0x00, 0x00, 0x20, 0x08, 0x40, 0x0f, 0x91, 0xc2, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7c, 0x31, 0xff,
    0x00, 0x00, 0x00, 0x08, 0x80, 0x10, 0x91, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc5, 0xff,
    0x00, 0x02, 0x00, 0x0c, 0x80, 0x20, 0x11, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x19, 0xff,
    0x00, 0x06, 0x00, 0x18, 0x00, 0x60, 0x31, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0xff,
    0x00, 0x06, 0x18, 0x18, 0x00, 0x41, 0x31, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x13, 0xff,
    0x00, 0x07, 0xb1, 0xb8, 0x00, 0xc1, 0x70, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x13, 0xff,
    0x00, 0x04, 0x31, 0xd8, 0x01, 0xc3, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x19, 0xff,
    0x00, 0x00, 0x30, 0xc9, 0xc3, 0xff, 0xf2, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1d, 0xff,
    0x00, 0x00, 0x30, 0xe9, 0xf3, 0xff, 0xf6, 0x21, 0xc0, 0x00, 0x00, 0x00, 0x64, 0x00, 0x3d, 0xff,
    0x00, 0x00, 0x30, 0x08, 0xf8, 0x00, 0x3e, 0x21, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xff,
    0x00, 0x00, 0x30, 0x08, 0x00, 0x00, 0x01, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xff,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9, 0xff,
    0x00, 0x00, 0x5f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xff,
    0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0xff,
    0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0xff,
    0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe7, 0xff,
    0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xff,
    0x00, 0x00, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9f, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x71, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0xff};

static const unsigned char PROGMEM small_lena[] =
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
    // Draw TobySoft 2.0 bootscreen
    display.drawBitmap(
        0,
        0,
        tobysoft_2, 128, 64, 1);

    display.display();
    delay(1000); // show for 4 seconds?

    display.clearDisplay();

    // Top bar
    display.fillRect(0, 16, 128, 48, SSD1306_INVERSE);

    // Draw Toby face bmp
    display.drawBitmap(
        (display.width() - 16) / 2,
        0,
        small_lena, 16, 16, 1);

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

// Show when autopilot not on
void displaySleepScreen()
{
    display.clearDisplay();
    display.drawBitmap(
        0,
        0,
        lena_lay, 128, 64, 1);

    display.display();
}
