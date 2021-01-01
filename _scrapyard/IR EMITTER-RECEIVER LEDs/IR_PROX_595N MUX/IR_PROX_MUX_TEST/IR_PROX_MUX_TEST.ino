
#define IRMUX_CLOCK_PIN 2
#define IRMUX_DATA_PIN 3
#define IRMUX_ANALOG_PIN A0 // data in

unsigned long lastShiftDelay = 0;
const int shiftDelay = 500;

bool clockPinSetting;

int cycleCount = 0;
int phaseCount = 0;

bool bitBool;


void setup() {
  Serial.begin(115200);
  
  //set pins
  pinMode(IRMUX_CLOCK_PIN, OUTPUT);
  pinMode(IRMUX_ANALOG_PIN, INPUT);

  digitalWrite(IRMUX_CLOCK_PIN, LOW);
}

void loop() {
  if (millis() - lastShiftDelay > shiftDelay) {
    lastShiftDelay = millis();

    if (cycleCount > 7) {
      cycleCount = 0;
      phaseCount += 1;

      if (phaseCount > 7) {
        phaseCount = 0;
      }

      if (cycleCount == phaseCount) {
        bitBool = true;
      } else {
        bitBool = false;
      }
    }
    
    if (clockPinSetting) {
      clockPinSetting = false;
      digitalWrite(IRMUX_CLOCK_PIN, LOW);
    } else {
      if (bitBool) {
        digitalWrite(IRMUX_DATA_PIN, HIGH);
      } else {
        digitalWrite(IRMUX_DATA_PIN, LOW);
      }
      clockPinSetting = true;
      digitalWrite(IRMUX_CLOCK_PIN, HIGH);
      cycleCount += 1;
      Serial.println(cycleCount);
    }
  } 
}
