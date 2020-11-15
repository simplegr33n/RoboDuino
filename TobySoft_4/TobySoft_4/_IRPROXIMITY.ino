#define IR_Prox_Pin_FL 44
#define IR_Prox_Pin_FC 45
#define IR_Prox_Pin_FR 46

void initIRProx()
{
    pinMode(IR_Prox_Pin_FL, INPUT);
    pinMode(IR_Prox_Pin_FC, INPUT);
    pinMode(IR_Prox_Pin_FR, INPUT);
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
}
