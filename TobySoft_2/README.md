[<img src="https://raw.githubusercontent.com/simplegr33n/RoboDuino/master/_assets/TS2.bmp" width="600">](https://github.com/simplegr33n/RoboDuino/tree/master/TobySoft_2)

# TobySoft 2.0

Arduino Mega based self driving car. Now with more crunchies.

## Sensors and Equipment

**L298N** - Fairly general purpose small motor controller

**SR04** (3x) - Front left, front middle, and front right array

**TOF10120** - Front facing IR time of flight distance sensor

**ADLX335** - Triple Axis Accelerometer _(currently only using x-axis, for collision detection)_

**SSD1306** - 128x64 OLED screen

**DFR0299** - DFPlayer Mini, a nifty little MP3 playing microcontroller _(and a small speaker for video game tunes)_

## Uno Pinout

| Pin              | _sensor_           | _sensor pin_ | _notes_                             |
| ---------------- | ------------------ | ------------ | ----------------------------------- |
| **_A0_**         |                    |              |
| **_A1_**         | Accelerometer      | _X-Axis_     |
| **_A2_**         |                    |              |
| **_A3_**         |                    |              |
| **_A4_**         |                    |              |
| **_A5_**         |                    |              |
| **_A6_**         |                    |              |
| **_A7_**         |                    |              |
| **_A8_**         |                    |              |
| **_A9_**         |                    |              |
| **_A10_**        |                    |              |
| **_A11_**        |                    |              |
| **_A12_**        |                    |              |
| **_A13_**        | HC-SR04 (F-Center) | _Echo_       | utilizing PCINT21                   |
| **_A14_**        | HC-SR04 (F-Left)   | _Echo_       | utilizing PCINT22                   |
| **_A15_**        | HC-SR04 (F-Right)  | _Echo_       | utilizing PCINT23                   |
| **_D0_** _RX0_   |                    |              |
| **_D1_** _TX0_   |                    |              |
| **_D2_**         |                    |              |
| **_D3_**         |                    |              |
| **_D4_**         |                    |              |
| **_D5_**         | L298N              |              |
| **_D6_**         | L298N              |              |
| **_D7_**         | L298N              |              |
| **_D8_**         | L298N              |              |
| **_D9_**         | L298N              |              |
| **_D10_**        | SR04 Trigger       | _Trig_       | single trigger pin for all SR04s    |
| **_D11_**        | L298N              |              |
| **_D12_**        | IR Receiver        | _Out_        |
| **_D13_**        |                    |              |
| **_D14_** _TX3_  |                    |              | USB COM Port                        |
| **_D15_** _RX3_  |                    |              | USB COM Port                        |
| **_D16_** _TX2_  | DFR0299            | _RX_         | \*9600 baud - 1k resistor for noise |
| **_D17_** _RX2_  | DFR0299            | _TX_         | \*9600 baud                         |
| **_D18_** _TX1_  |                    |              |
| **_D19_** _RX1_  |                    |              |
| **_D20_** _II2C_ | OLED (SSD1306)     | _SDA_        |
|                  | TOF10120           | _SDA_        |
| **_D21_** _II2C_ | OLED (SSD1306)     | _SCL_        |
|                  | TOF10120           | _SCL_        |
| **_D22_**        |                    |              |
| **_D23_**        |                    |              |
| **_D24_**        |                    |              |
| **_D25_**        |                    |              |
| **_D26_**        |                    |              |
| **_D27_**        |                    |              |
| **_D28_**        |                    |              |
| **_D29_**        |                    |              |
| **_D30_**        |                    |              |
| **_D31_**        |                    |              |
| **_D32_**        |                    |              |
| **_D33_**        |                    |              |
| **_D34_**        |                    |              |
| **_D35_**        |                    |              |
| **_D36_**        |                    |              |
| **_D37_**        |                    |              |
| **_D38_**        |                    |              |
| **_D39_**        |                    |              |
| **_D40_**        |                    |              |
| **_D41_**        |                    |              |
| **_D42_**        |                    |              |
| **_D43_**        |                    |              |
| **_D44_**        |                    |              |
| **_D45_**        |                    |              |
| **_D46_**        |                    |              |
| **_D47_**        |                    |              |
| **_D48_**        |                    |              |
| **_D49_**        |                    |              |
| **_D50_**        |                    |              |
| **_D51_**        |                    |              |
| **_D52_**        |                    |              |
| **_D53_**        |                    |              |
