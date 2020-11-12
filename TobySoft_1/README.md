[<img src="https://raw.githubusercontent.com/simplegr33n/RoboDuino/master/_assets/tobylogo2.bmp" width="600">](https://github.com/simplegr33n/RoboDuino/tree/master/TobySoft_1)

# TobySoft 1.0

Arduino Uno based self driving car. Documentation to allow myself a rebuild, as the project is being torn down and expanded to a Mega for TobySoft 2.0. While there are some pins remaining on the Uno, utilizing the DFPlayer requires use of the sole hardware Serial port which interferes with my ability to log.

Beyond having 4 hardware Serial ports to the Uno's 1, other advantages to the Mega include additional interrupt pins (though I got around that to this point by using pin change interrupts), additional flash memory and SRAM, 40 additional digital pins, 9 additional PWM enabled pins, and 10 additional analog pins. I doubt I will be able to utilize this board quite to the extent that I have with the Uno before needing additional processing power to achieve my goals (considering a Beagleboard or Jetson Nano as I try to implement computer vision).

## Sensors and Equipment

**L298N** - Fairly general purpose small motor controller

**SR04** (3x) - Front left, front middle, and front right array

**TOF10120** - Front facing IR time of flight distance sensor

**ADLX335** - Triple Axis Accelerometer _(hooked up, but not utilized in TobySoft 1.0)_

**SSD1306** - 128x64 OLED screen

**DFR0299** - DFPlayer Mini, a nifty little MP3 playing microcontroller _(and a small speaker for video game tunes)_

## Uno Pinout

| Pin             | _sensor_       | _sensor pin_ | _notes_                                   |
| --------------- | -------------- | ------------ | ----------------------------------------- |
| **_A0_**        |                |              |
| **_A1_**        | Accelerometer  | _X-Axis_     |
| **_A2_**        |                |              |
| **_A3_**        |                |              |
| **_A4_** _II2C_ | OLED (SSD1306) | _SDA_        |
|                 | TOF10120       | _SDA_        |
| **_A5_** _II2C_ | OLED (SSD1306) | _SCL_        |
|                 | TOF10120       | _SCL_        |
| **_D0_** _(TX)_ | DFR0299        | _RX_         | use of 1k resistor to reduce noise        |
| **_D1_** _(RX)_ | DFR0299        | _TX_         |
| **_D2_**        | Left SR04      | _Echo_       | pin change interrupt used for echo timing |
| **_D3_**        | Center SR04    | _Echo_       | pin change interrupt used for echo timing |
| **_D4_**        | Right SR04     | _Echo_       | pin change interrupt used for echo timing |
| **_D5_**        | L298N          |              |
| **_D6_**        | L298N          |              |
| **_D7_**        | L298N          |              |
| **_D8_**        | L298N          |              |
| **_D9_**        | L298N          |              |
| **_D10_**       | SR04 Trigger   | _Trig_       | single trigger pin for all SR04s          |
| **_D11_**       | L298N          |              |
| **_D12_**       | IR Receiver    | _Out_        |
| **_D13_**       | Onboard LED    |              |
