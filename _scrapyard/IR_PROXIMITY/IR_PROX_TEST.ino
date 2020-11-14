#define IR_Prox_Pin 46
int inputVal = 0;

void setup()
{
    pinMode(IR_Prox_Pin, INPUT);
    Serial.begin(9600);
}

void loop()
{
    inputVal = digitalRead(IR_Prox_Pin);
    Serial.println(inputVal);
    delay(1000); // wait for a second
}
