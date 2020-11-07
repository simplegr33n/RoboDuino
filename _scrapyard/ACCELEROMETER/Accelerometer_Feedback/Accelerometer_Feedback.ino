#define xPin A1 // x-axis of the accelerometer

int accelerometerSampleCount = 0;
int accelerometerAffirmCount = 0;
unsigned long lastAccelerometerSample = 0;

String DRIVE_STATE = "FORWARD";
bool IS_STUCK = false;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if ((lastAccelerometerSample == 0) || (micros() - lastAccelerometerSample > 30000))
    {
        lastAccelerometerSample = micros();
        sampleAccelerometer();
    }

    if (IS_STUCK == false)
    {
        Serial.println("GOOD!");
    }
    else
    {
        Serial.println("STUCK!");
    }
}

void sampleAccelerometer(void)
{
    int x = analogRead(xPin); //read from xpin

    if ((DRIVE_STATE == "STOPPED") || (((((float)x - 331.5) / 65 * 9.8) > 0.8) || ((((float)x - 331.5) / 65 * 9.8) < -0.8)))
    {
        accelerometerAffirmCount++;
    }

    accelerometerSampleCount++;

    if (accelerometerSampleCount == 7)
    {
        if (accelerometerAffirmCount < 6)
        {
            IS_STUCK = true;
        }
        else
        {
            IS_STUCK = false;
        }

        accelerometerSampleCount = 0;
        accelerometerAffirmCount = 0;
    }
}