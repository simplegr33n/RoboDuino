//www.elegoo.com

#include <IRremote.h>

//------- IR REMOTE CODES ---------//
#define FWD 16736925           // FORWARD
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

void forward()
{
  Serial.println("go forward!");
}

void back()
{
  Serial.println("go back!");
}

void left()
{
  Serial.println("go left!");
}

void right()
{
  Serial.println("go right!");
}

void stop()
{
  Serial.println("STOP!");
}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop()
{
  if (irrecv.decode(&results))
  {
    preMillis = millis();
    val = results.value;
    Serial.println(val);
    irrecv.resume();
    switch (val)
    {
    case FWD:
      forward();
      break;
    case B:
      back();
      break;
    case L:
      left();
      break;
    case R:
      right();
      break;
    case S:
      stop();
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
}
