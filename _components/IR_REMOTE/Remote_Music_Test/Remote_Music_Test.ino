//www.elegoo.com

#include <IRremote.h>

//------- IR REMOTE CODES ---------//
#define FWD 16736925         // FORWARD
#define B 16754775           // BACK
#define L 16720605           // LEFT
#define R 16761405           // RIGHT
#define S 16712445           // STOP
#define UNKNOWN_F 5316027    // FORWARD
#define UNKNOWN_B 2747854299 // BACK
#define UNKNOWN_L 1386468383 // LEFT
#define UNKNOWN_R 553536955  // RIGHT
#define UNKNOWN_S 3622325019 // STOP
#define KEY1 16738455
#define KEY2 16750695
#define KEY3 16756815
#define KEY4 16724175
#define KEY5 16718055
#define KEY6 16743045
#define KEY7 16716015
#define KEY8 16726215
#define KEY9 16734885
#define KEY0 16730805
#define KEY_STAR 16728765
#define KEY_HASH 16732845

#define RECV_PIN 12

IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long val;
unsigned long preMillis;

///////////////////
// For the music...
///////////////////
#define BUZZER1 A0

// TONES  ==========================================
#include "pitches.h"
bool playTriggered = false;

// Define a special note, 'R', to represent a rest
#define REST 0
#define REST_COUNT 185

#define TEMPO 35000
#define PAUSE 1000 // length of pause between notes

/**************************************************************************
MUSIC and stuff below
 **************************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT 64
#define LOGO_WIDTH 128
const unsigned char PROGMEM lena_bmp[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xe3, 0xb3, 0x33, 0x30, 0xf8, 0xf3, 0x07, 0xe3, 0x0d, 0x9b, 0x38, 0xd9, 0xb0, 0xff, 0xff, 0xff,
    0xc9, 0xb2, 0x32, 0x30, 0xf8, 0x73, 0x87, 0xc1, 0x65, 0x99, 0x30, 0x59, 0xb0, 0xff, 0xfc, 0x07,
    0xdf, 0xb2, 0x32, 0x30, 0xfb, 0x21, 0xcf, 0x9f, 0x65, 0x98, 0x27, 0xc1, 0xb0, 0xff, 0xf8, 0x03,
    0x99, 0xb0, 0x30, 0x30, 0xfb, 0x25, 0xcf, 0x9f, 0x0d, 0x98, 0x27, 0xc1, 0xb0, 0xff, 0xe0, 0x01,
    0xdd, 0xb4, 0xb4, 0xb7, 0xfb, 0x20, 0xcf, 0xd9, 0x6c, 0x9a, 0x36, 0x59, 0xb7, 0xff, 0xe1, 0xc1,
    0xc1, 0xb5, 0xb5, 0xb0, 0xf8, 0x4c, 0xcf, 0xc3, 0x64, 0x3b, 0x30, 0xd9, 0xb0, 0xff, 0xc1, 0xc1,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xc1,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xc1,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0x0f, 0xc1,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x7f, 0x0f, 0x81,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x04, 0x7e, 0x0f, 0x81,
    0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1c, 0x7e, 0x0f, 0x83,
    0xd6, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd0, 0x54, 0x1f, 0x81,
    0xee, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xfc, 0x1e, 0x07,
    0x4e, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x41, 0x54, 0x1e, 0x05,
    0xee, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xfc, 0x1c, 0x0f,
    0x4e, 0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x05, 0x54, 0x00, 0x15,
    0xee, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0x8c, 0x00, 0x3f,
    0x4e, 0x05, 0x55, 0x55, 0x55, 0x51, 0x55, 0x55, 0x55, 0x55, 0x55, 0x50, 0x01, 0x05, 0x00, 0x55,
    0xee, 0x07, 0xff, 0xff, 0xff, 0xe6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x03, 0x1f, 0x01, 0xff,
    0xce, 0x01, 0x05, 0xd5, 0xd5, 0xc5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5,
    0xef, 0x00, 0x07, 0xff, 0xff, 0x8d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x4f, 0x80, 0x00, 0x10, 0x54, 0x11, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0xef, 0x87, 0x81, 0x00, 0x00, 0x3b, 0x80, 0x00, 0x3f, 0xe0, 0x3f, 0xf8, 0x01, 0xfe, 0x1f, 0xe0,
    0x56, 0x38, 0x05, 0x98, 0x00, 0x11, 0x00, 0x00, 0x15, 0x00, 0x05, 0x50, 0x00, 0x14, 0x05, 0x41,
    0xf4, 0x70, 0x85, 0x88, 0x00, 0x3b, 0x80, 0x00, 0x3e, 0x00, 0x03, 0xf8, 0x00, 0x3e, 0x0f, 0xc1,
    0x50, 0xe0, 0xc4, 0x9a, 0x00, 0x15, 0x54, 0x05, 0x54, 0x15, 0x01, 0x50, 0x54, 0x15, 0x05, 0x41,
    0xf1, 0xf0, 0xc4, 0xf6, 0x04, 0x37, 0xfe, 0x0f, 0xf8, 0x3f, 0xc1, 0xf8, 0x7e, 0x1f, 0x07, 0x83,
    0xd1, 0xaf, 0xce, 0xf6, 0x67, 0x05, 0xd4, 0x05, 0xd0, 0x55, 0xc0, 0xd0, 0x54, 0x15, 0x85, 0x85,
    0xf3, 0x80, 0xea, 0xf6, 0x63, 0x1f, 0xfe, 0x0f, 0xf0, 0x7f, 0xe0, 0xf8, 0x7e, 0x1f, 0x83, 0x07,
    0x44, 0x67, 0x39, 0xf6, 0x61, 0x95, 0x54, 0x05, 0x50, 0x55, 0x50, 0x50, 0x54, 0x15, 0x41, 0x05,
    0xcb, 0xce, 0x59, 0xf7, 0xf8, 0x1f, 0xfe, 0x0f, 0xf0, 0xff, 0xf0, 0xf8, 0x7c, 0x3f, 0xc1, 0x0f,
    0x3b, 0xce, 0x5d, 0x77, 0xc6, 0x15, 0x54, 0x05, 0x50, 0x55, 0x50, 0x50, 0x00, 0x55, 0x40, 0x15,
    0xb3, 0x26, 0xed, 0x76, 0xf3, 0x3f, 0xfe, 0x0f, 0xf0, 0xff, 0xf0, 0x78, 0x00, 0x3f, 0xe0, 0x1f,
    0x67, 0x82, 0xeb, 0xf5, 0x96, 0x15, 0x54, 0x05, 0x50, 0x55, 0x50, 0x50, 0x00, 0x15, 0x50, 0x15,
    0x4e, 0xf0, 0x02, 0xf1, 0x9f, 0x9f, 0xfe, 0x0f, 0xf0, 0xff, 0xf0, 0xf8, 0x7c, 0x0f, 0xf0, 0x3f,
    0x1e, 0x00, 0x02, 0xf1, 0x99, 0xc5, 0xd4, 0x05, 0xd0, 0xd5, 0xd0, 0xd0, 0x55, 0x05, 0xd0, 0x55,
    0x3f, 0xe8, 0x00, 0x10, 0x80, 0x0f, 0xfe, 0x0f, 0xf0, 0xff, 0xf0, 0xf8, 0x7f, 0x07, 0xf8, 0x7f,
    0x7f, 0xe6, 0x01, 0x00, 0x02, 0x05, 0x54, 0x05, 0x50, 0x55, 0x40, 0x50, 0x55, 0x05, 0x50, 0x55,
    0x7c, 0x0e, 0x80, 0xf0, 0x01, 0x87, 0xfe, 0x0f, 0xf0, 0x7f, 0xe1, 0xf8, 0x7f, 0x07, 0xf8, 0x7f,
    0x39, 0xdf, 0x4c, 0x43, 0xf4, 0x05, 0x54, 0x05, 0x50, 0x15, 0x41, 0x50, 0x55, 0x05, 0x50, 0x55,
    0x03, 0xdd, 0x7e, 0xcf, 0xfb, 0xc3, 0xfe, 0x0f, 0xf8, 0x1f, 0x83, 0xf8, 0x7e, 0x0f, 0xf8, 0x7f,
    0x4f, 0xdf, 0x5e, 0x5e, 0xf9, 0x41, 0x54, 0x05, 0x54, 0x00, 0x05, 0x50, 0x00, 0x15, 0x50, 0x55,
    0x6f, 0xcd, 0xff, 0x3f, 0x98, 0xfb, 0xfe, 0x0f, 0xff, 0x00, 0x0f, 0xf8, 0x00, 0x3f, 0xf8, 0x7f,
    0x27, 0xcf, 0x5f, 0x7d, 0x5a, 0x15, 0xd5, 0x05, 0xd5, 0xc0, 0x55, 0xd0, 0x00, 0xd5, 0xd0, 0x55,
    0x33, 0xa5, 0xff, 0x7b, 0x1b, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3,
    0x39, 0xb1, 0x5c, 0x3e, 0xf3, 0x85, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x11,
    0x9c, 0xb8, 0x40, 0x07, 0x63, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x2a, 0xab,
    0x0e, 0x5e, 0x0f, 0xc9, 0xe0, 0x65, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x44, 0x41,
    0x86, 0x6f, 0xff, 0xe6, 0xf9, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xaa, 0xaa, 0xab,
    0x47, 0x37, 0xff, 0xf1, 0xb3, 0x95, 0x55, 0x55, 0x55, 0x55, 0x54, 0x00, 0x01, 0x01, 0x01, 0x01,
    0xc3, 0x9d, 0xff, 0xfe, 0xe3, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0xab,
    0xc1, 0xde, 0x7f, 0xff, 0x86, 0x15, 0xd5, 0xd5, 0xd5, 0xd5, 0x00, 0x44, 0x44, 0x44, 0x44, 0x41,
    0xe0, 0xef, 0x9f, 0xff, 0x4e, 0x7f, 0xff, 0xff, 0xff, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xab,
    0x50, 0x27, 0xe7, 0xf8, 0x80, 0x55, 0x55, 0x55, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0xf8, 0x01, 0xf9, 0xff, 0x01, 0xff, 0xff, 0xfe, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xab,
    0x50, 0x00, 0x7f, 0xfc, 0x05, 0x55, 0x54, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x41,
    0xfc, 0x00, 0x1f, 0xe0, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
    0x55, 0x00, 0x00, 0x00, 0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};

/// MUSIC stuff
// MELODY and TIMING  =======================================
// notes in the melody 1
int melody1[] = {
    NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_GS3, NOTE_AS3,
    NOTE_C3, NOTE_AS3, NOTE_C3};

// note durations: 1 = 1/16 note, 3 = 3/16 note, etc.:
int beats1[] = {
    1, 1, 1, 3, 3, 3,
    2, 1, 9};

void setup()
{
  Serial.begin(9600);

  irrecv.enableIRIn();

  pinMode(BUZZER1, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  displayHeader();
}

void loop()
{
  // REMOTE Decode
  if (irrecv.decode(&results))
  {
    preMillis = millis();
    val = results.value;
    Serial.println(val);
    irrecv.resume();
    switch (val)
    {
    case FWD:
      playTriggered = true;
      break;
    default:
      break;
    }
  }
  else
  {
    if (millis() - preMillis > 500)
    {
      preMillis = millis();
    }
  }

  if (playTriggered)
  {
    int n = sizeof(beats1) / 2;
    for (int i = 0; i < n; i++)
    {
      playTone(melody1[i], beats1[i] * 4 * TEMPO);
    }
    playTriggered = false;
  }
}

void displayHeader(void)
{
  display.clearDisplay();

  // Toby face
  display.drawBitmap(
      (display.width() - LOGO_WIDTH) / 2,
      0,
      lena_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);

  display.display();
}

void rest(long duration)
{
  for (int j = 0; j < REST_COUNT; j++)
  {
    delayMicroseconds(duration);
  }
}

void playTone(long tone_, long duration)
{
  byte state = LOW;
  long elapsed_time = 0;
  if (tone_ > 0)
  {
    tone_ >>= 1;
    while (elapsed_time < duration)
    {
      state = state == HIGH ? LOW : HIGH;
      digitalWrite(BUZZER1, state);
      delayMicroseconds(tone_);
      elapsed_time += tone_;
    }
  }
  else
    rest(duration);
}
