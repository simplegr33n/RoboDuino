[<img src="https://raw.githubusercontent.com/simplegr33n/RoboDuino/master/_assets/TS5.bmp" width="600">](https://github.com/simplegr33n/RoboDuino/tree/master/TobySoft_5)

# TobySoft 6.0

Advanced TobySoft and moved TobyTransmit to its own project. The Elegoo robot has officially been retired. New TobyMax-Wide base has been printed and development underway. Intending to round off code for the base Arduino(s) so as to allow for moving onto Jetson development (likely TobySoft 7.0).

- NOTE: The Toby-Max Wide v1 robot will likely be revised to use a Raspberry Pi 3b+ I have lying around and a Arduino Nano.

## TobyMax-Wide

Printed at the max bed size for a Prusa MK3, and driving with it's long face forward, this computer case with wheels has a lot of work to do.

## Sensors and Electronics

**Arduino Mega** - running an Atmega 2560. For the robot sensors.

**Arduino Nano** - running an Atmega 328P. For driving the motors.

**NRF24L01** - RF Transceiver module.

**L298N** - Fairly general purpose small motor controller.

**TOF10120** - Front facing IR time of flight distance sensor

**IR Proximity Sensor** (9x?) - Simple and cheap digital obstacle detector, _5x_ in the front, and _2x_ rear, and _4x_ downfacing for checking for cliffs at each corner/tirepoint.

**Optical Mouse Sensor** - to confirm robot movement.

**ADLX335** - Triple Axis Accelerometer _(currently only using x-axis, for collision detection)_

**DFR0299** - DFPlayer Mini, a nifty little MP3 playing microcontroller _(and a small speaker for video game tunes)_

## Mega Pinout

| Pin                | _sensor_                  | _sensor pin_ | _notes_                                  |
| ------------------ | ------------------------- | ------------ | ---------------------------------------- |
| **_A0_**           |                           |              |
| **_A1_**           | Accelerometer             | _X-Axis_     |
| **_A2_**           |                           |              |
| **_A3_**           |                           |              |
| **_A4_**           |                           |              |
| **_A5_**           |                           |              |
| **_A6_**           |                           |              |
| **_A7_**           |                           |              |
| **_A8_**           |                           |              |
| **_A9_**           |                           |              |
| **_A10_**          |                           |              |
| **_A11_**          |                           |              |
| **_A12_**          |                           |              |
| **_A13_**          |                           |              |                                          |
| **_A14_**          |                           |              |                                          |
| **_A15_**          |                           |              |                                          |
| **_D0_** - _RX0_   | USB COM PORT              | USB COM PORT | USB COM PORT                             |
| **_D1_** - _TX0_   | USB COM PORT              | USB COM PORT | USB COM PORT                             |
| **_D2_**           |                           |              |
| **_D3_**           |                           |              |
| **_D4_**           |                           |              |
| **_D5_**           | L298N                     |              |
| **_D6_**           | L298N                     |              |
| **_D7_**           | L298N                     |              |
| **_D8_**           | L298N                     |              |
| **_D9_**           | L298N                     |              |
| **_D10_**          |                           |              |                                          |
| **_D11_**          | L298N                     |              |
| **_D12_**          | IR Receiver               | _Out_        |
| **_D13_**          |                           |              |
| **_D14_** - _TX3_  |                           |              |                                          |
| **_D15_** - _RX3_  |                           |              |                                          |
| **_D16_** - _TX2_  | DFR0299                   | _RX_         | \*9600 baud - 1k resistor for noise      |
| **_D17_** - _RX2_  | DFR0299                   | _TX_         | \*9600 baud                              |
| **_D18_** - _TX1_  |                           |              |
| **_D19_** - _RX1_  |                           |              |
| **_D20_** - _II2C_ | TOF10120                  | _SDA_        |
| **_D21_** - _II2C_ | TOF10120                  | _SCL_        |
| **_D22_**          |                           |              |
| **_D23_**          |                           |              |
| **_D24_**          |                           |              |
| **_D25_**          |                           |              |
| **_D26_**          |                           |              |
| **_D27_**          |                           |              |
| **_D28_**          |                           |              |
| **_D29_**          |                           |              |
| **_D30_**          |                           |              |
| **_D31_**          |                           |              |
| **_D32_**          |                           |              |
| **_D33_**          |                           |              |
| **_D34_**          |                           |              |
| **_D35_**          |                           |              |
| **_D36_**          |                           |              |
| **_D37_**          |                           |              |
| **_D38_**          |                           |              |
| **_D39_**          |                           |              |
| **_D40_**          |                           |              |
| **_D41_**          |                           |              |
| **_D42_**          | Front-Left IR-Drop        | _Out_        |
| **_D43_**          | Front-Right IR-Drop       | _Out_        |
| **_D44_**          | Front-Left IR-Proximity   | _Out_        |
| **_D45_**          | Front-Center IR-Proximity | _Out_        |
| **_D46_**          | Front-Right IR-Proximity  | _Out_        |                                          |
| **_D47_**          |                           |              |
| **_D48_**          | NRF24L01                  | _CE_         | \*1000µF capacitor over sensor VCC + GnD |
| **_D49_**          | NRF24L01                  | _CSN_        |
| **_D50_** - _MISO_ | NRF24L01                  | _MISO_       |
| **_D51_** - _MOSI_ | NRF24L01                  | _MOSI_       |
| **_D52_** - _SCK_  | NRF24L01                  | _SCK_        |
| **_D53_**          |                           |              |
