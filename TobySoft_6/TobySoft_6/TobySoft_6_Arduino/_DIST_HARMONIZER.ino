/////////////////////////////////////////////////////////////////////////////////////////////////////////
// For TOF10120                                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Wire.h>
//
int tofReadDistance = 0;
unsigned long lastTOFRead = 0; // micros() timestamp of last TOF10120 read
//
unsigned char ok_flag;
unsigned char fail_flag;
//
unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];
unsigned char dirsend_flag = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR-Prox sensors                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int irProxThreshold = 650;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR-Prox Front sensors                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IR_Prox_Pin_FR A0
#define IR_Prox_Pin_FL A1
#define IR_Prox_Pin_FCR A2
#define IR_Prox_Pin_FCL A3
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR-Prox TODO: Side sensors                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define IR_Drop_Pin_R ??
// #define IR_Drop_Pin_L ??
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR-Prox Back sensors                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define IR_Prox_Pin_BR ??
// #define IR_Prox_Pin_BL ??
#define IR_Prox_Pin_BCR A6
#define IR_Prox_Pin_BCL A7
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR-Prox TODO: Drop sensors                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define IR_Drop_Pin_FR ??
// #define IR_Drop_Pin_FL ??
// #define IR_Drop_Pin_BR ??
// #define IR_Drop_Pin_BL ??

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEBUG                                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long lastDistPrint;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initDistanceSensors()
{
    Serial.println("TobySoft Distance Sensors initiating");

    // Init II2C for TOF10120
    Wire.begin(); // for TOF on II2C

    // Init IR-Prox sensors
    pinMode(IR_Prox_Pin_FR, INPUT);
    pinMode(IR_Prox_Pin_FL, INPUT);
    pinMode(IR_Prox_Pin_FCR, INPUT);
    pinMode(IR_Prox_Pin_FCL, INPUT);
    pinMode(IR_Prox_Pin_BCR, INPUT);
    pinMode(IR_Prox_Pin_BCL, INPUT);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Distance Check functions                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void getDistances()
{
    // Read TOF and IR-Prox sensors
    readTOFDistance();
    checkIRProx();

    // Harmonize readings
    harmonizeDistances();
}

void harmonizeDistances()
{
    // Get best Front-Center distance
    if ((irProxValueFCR > irProxThreshold) || (irProxValueFCL > irProxThreshold))
    {
        frontCenterDistance = 1;
    }
    else
    {
        //        frontCenterDistance = tofReadDistance;
        frontCenterDistance = 99;
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
// TOF10120 Read functions                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void readTOFDistance()
{
    if (micros() - lastTOFRead > 20000)
    {
        lastTOFRead = micros();

        TOFSensorRead(0x00, i2c_rx_buf, 2);
        lenth_val = i2c_rx_buf[0];
        lenth_val = lenth_val << 8;
        lenth_val |= i2c_rx_buf[1];

        tofReadDistance = (int)lenth_val / 10; // get centimeter value to harmonize with ultrasonics
    }
}

void TOFSensorRead(unsigned char addr, unsigned char *datbuf, unsigned char cnt)
{
    unsigned short result = 0;
    // step 1: instruct sensor to read echoes
    Wire.beginTransmission(82); // transmit to device #82 (0x52)
    // the address specified in the datasheet is 164 (0xa4)
    // but i2c adressing uses the high 7 bits so it's 82
    Wire.write(byte(addr)); // sets distance data address (addr)
    Wire.endTransmission(); // stop transmitting
    // step 2: wait for readings to happen
    delayMicroseconds(100); // datasheet suggests at least 30uS
    // step 3: request reading from sensor
    Wire.requestFrom(82, (int)cnt); // request cnt bytes from slave device #82 (0x52)
    // step 5: receive reading from sensor
    if (cnt <= Wire.available())
    {                            // if two bytes were received
        *datbuf++ = Wire.read(); // receive high byte (overwrites previous reading)
        *datbuf++ = Wire.read(); // receive low byte as lower 8 bits
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

    // TODO: Check drops
}
