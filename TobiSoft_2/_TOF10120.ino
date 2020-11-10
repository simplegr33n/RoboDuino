/*
  TOF10120 Distance Sensor

   ##II2C connection
     # BLUE wire - A4
     # GREEN wire - A5

  Adapted from code from DroneBot Workshop 2019
  https://dronebotworkshop.com
*/

#include <Wire.h>

unsigned char ok_flag;
unsigned char fail_flag;

unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];
unsigned char dirsend_flag = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void initTOF10120()
{
    Wire.begin();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TOF Read functions                                                                                  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
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

void readTOFDistance()
{
    TOFSensorRead(0x00, i2c_rx_buf, 2);
    lenth_val = i2c_rx_buf[0];
    lenth_val = lenth_val << 8;
    lenth_val |= i2c_rx_buf[1];

    tofReadDistance = (int)lenth_val / 10; // get centimeter value to harmonize with ultrasonics
}
