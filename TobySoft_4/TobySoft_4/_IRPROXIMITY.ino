// Low front facing IRs for rapid close range low object detection
#define IR_Prox_Pin_FL 44
#define IR_Prox_Pin_FC 45
#define IR_Prox_Pin_FR 46

// IR Prox sensors facing down to detect dropoffs
#define IR_Drop_Pin_FL 42
#define IR_Drop_Pin_FR 43

void initIRProx()
{
    pinMode(IR_Prox_Pin_FL, INPUT);
    pinMode(IR_Prox_Pin_FC, INPUT);
    pinMode(IR_Prox_Pin_FR, INPUT);

    pinMode(IR_Drop_Pin_FL, INPUT);
    pinMode(IR_Drop_Pin_FR, INPUT);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check IR Proximity functions                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void checkIRProx()
{
    // Check Front-Left/Center/Right promixities
    irProxValueFL = digitalRead(IR_Prox_Pin_FL);
    irProxValueFC = digitalRead(IR_Prox_Pin_FC);
    irProxValueFR = digitalRead(IR_Prox_Pin_FR);

    // Check front drop values
    irDropValueFL = digitalRead(IR_Drop_Pin_FL);
    irDropValueFR = digitalRead(IR_Drop_Pin_FR);
}
