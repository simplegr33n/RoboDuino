unsigned long lastProxIR;

void setup() {
  Serial.begin(112500);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
}

void loop() {
  if (millis() - lastProxIR > 1000) {
    lastProxIR = millis();
    
    Serial.print("AO: ");
    Serial.print(analogRead(A0));
    Serial.print(", A1: ");
    Serial.print(analogRead(A1));
    Serial.print(", A2: ");
    Serial.print(analogRead(A2));
    Serial.print(", A3: ");
    Serial.print(analogRead(A3));
    Serial.print(", A6: ");
    Serial.print(analogRead(A6));
    Serial.print(", A7: ");
    Serial.println(analogRead(A7));
  }
}
