/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR-Prox sensors                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int irProxThreshold = 600;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEBUG                                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long lastDistPrint;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     ///////////////                                                 //
//                                     END GLOBAL VARS                                                 //
//                                     ///////////////                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ======================================== INIT ===================================================== //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void initProxWatcher()
{
    Serial.println("TobySoft Distance Sensors initiating");

    // Init IR-Prox sensors
    pinMode(IR_Prox_Pin_FR, INPUT);
    pinMode(IR_Prox_Pin_FL, INPUT);
    pinMode(IR_Prox_Pin_FCR, INPUT);
    pinMode(IR_Prox_Pin_FCL, INPUT);
    pinMode(IR_Prox_Pin_BCR, INPUT);
    pinMode(IR_Prox_Pin_BCL, INPUT);

    // Setup Sharp-IR
    analogWrite(SHARP_IR_PIN, LOW);
    pinMode(SHARP_IR_PIN, INPUT);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Distance Check functions                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void getDistances()
{
    // Check IR-Prox sensors
    checkIRProx();

    // Harmonize readings
    harmonizeDistances();
}

void harmonizeDistances()
{
    int tempSharpIR = getSharpIR();
    
    // Get best Front-Center distance
    if ((irProxValueFCR > irProxThreshold) || (irProxValueFCL > irProxThreshold))
    {
        frontCenterDistance = 1;
    }
    else
    {
        if (tempSharpIR < 99) {
            frontCenterDistance = tempSharpIR;
        } else {
            frontCenterDistance = 99;
        }
    }

    // Get best Front-Left distance
    if ((irProxValueFL > irProxThreshold) || (irProxValueFCL > irProxThreshold))
    {
        frontLeftDistance = 1;
    }
    else
    {
        frontLeftDistance = 99;
    }

    // Get best Front-Right distance
    if ((irProxValueFR > irProxThreshold) || (irProxValueFCR > irProxThreshold))
    {
        frontRightDistance = 1;
    }
    else
    {
        frontRightDistance = 99;
    }

    // Get best Back distance
    if ((irProxValueBCL > irProxThreshold) || (irProxValueBCR > irProxThreshold))
    {
        backDistance = 1;
    }
    else
    {
        backDistance = 99;
    }

    // DEBUG
    if (millis() - lastDistPrint > toggleDebounce)
    {
        lastDistPrint = millis();

        Serial.print("Front Left: ");
        Serial.print(frontLeftDistance);
        Serial.print("Front Center: ");
        Serial.print(frontCenterDistance);
        Serial.print("Front Right: ");
        Serial.println(frontRightDistance);
        Serial.print("Back: ");
        Serial.println(backDistance);

        Serial.println("========================");
        Serial.println(irProxValueFCR);
        Serial.println(irProxValueFCL);
        Serial.println(irProxValueFR);
        Serial.println(irProxValueFL);
        Serial.println(irProxValueBCR);
        Serial.println(irProxValueBCL);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR-Prox Read functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void checkIRProx()
{
    // Check promixities
    irProxValueFR = analogRead(IR_Prox_Pin_FR);
    irProxValueFL = analogRead(IR_Prox_Pin_FL);
    irProxValueFCR = analogRead(IR_Prox_Pin_FCR);
    irProxValueFCL = analogRead(IR_Prox_Pin_FCL);
    irProxValueBCR = analogRead(IR_Prox_Pin_BCR);
    irProxValueBCL = analogRead(IR_Prox_Pin_BCL);

    //// TODO: Check drops
}

int getSharpIR()
{
    float sharpVolts = analogRead(SHARP_IR_PIN) * 0.0048828125; // Sensor value * (5v/1024bits)
    int sharpDist = 29.988 * pow(sharpVolts, -1.173);

    return sharpDist;
}
