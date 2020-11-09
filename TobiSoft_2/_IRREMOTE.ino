#include <IRremote.h>

//------- IR REMOTE CODES ---------//
#define FWD 16736925         // FORWARD
#define B 16754775           // BACK
#define L 16720605           // LEFT
#define R 16761405           // RIGHT
#define S 16712445           // STOP
#define UNKNOWN_F 5316027    // FORWARD (secondary code?)
#define UNKNOWN_B 2747854299 // BACK (secondary code?)
#define UNKNOWN_L 1386468383 // LEFT (secondary code?)
#define UNKNOWN_R 553536955  // RIGHT (secondary code?)
#define UNKNOWN_S 3622325019 // STOP (secondary code?)
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
decode_results IR_REMOTE_RESULTS;
unsigned long IR_RemoteSignalValue;
unsigned long IR_RemotePreMillis;

void initIR_Remote()
{
  irrecv.enableIRIn();
}

void checkIR_Remote()
{
  if (irrecv.decode(&IR_REMOTE_RESULTS))
  {
    IR_RemotePreMillis = millis();
    IR_RemoteSignalValue = IR_REMOTE_RESULTS.value;
    irrecv.resume();
    switch (IR_RemoteSignalValue)
    {
    case KEY7:
      // Play FF7 folder
      if (MUSIC_ON == true)
      {
        myDFPlayer.pause();
        MUSIC_ON = false;
      }
      else
      {
        myDFPlayer.randomAll();
        MUSIC_ON = true;
      }
      break;
    case KEY_STAR:
      // Start/Stop autonomous function
      if (AUTOPILOT_ON == true)
      {
        stopCar();
        AUTOPILOT_ON = false;
      }
      else
      {
        AUTOPILOT_ON = true;
      }
      break;

      // Driving
    case FWD:
      forward();
      break;
    case B:
      reverse();
      break;
    case L:
      left();
      break;
    case R:
      right();
      break;
    case S:
      Serial.println("STOP!");
      stopCar();
      break;
    default:
      break;
    }
  }
  else
  {
    if (millis() - IR_RemotePreMillis > 500)
    {
      IR_RemotePreMillis = millis();
    }
  }
}
