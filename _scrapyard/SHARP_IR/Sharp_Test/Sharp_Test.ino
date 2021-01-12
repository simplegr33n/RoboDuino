///////////////////
// For the SHARP
///////////////////
#define SHARP_IR_PIN A6      // define signal pin

unsigned long lastSharpMeasurement;

void setup()
{
  Serial.begin(115200);
  analogWrite(SHARP_IR_PIN, LOW);
  pinMode(SHARP_IR_PIN, INPUT);
}

void loop()
{
  if (millis() - lastSharpMeasurement > 500)
  {
    lastSharpMeasurement = millis();

    float volts = analogRead(SHARP_IR_PIN)*0.0048828125; // Sensor value * (5v/1024bits)
    int dist = 29.988*pow(volts, -1.173);

    Serial.print("Volts: ");
    Serial.println(volts);

    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.println("cm");
  }
}
