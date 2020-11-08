[<img src="https://raw.githubusercontent.com/simplegr33n/RoboDuino/master/_assets/tobylogo2.bmp" width="600">](https://github.com/simplegr33n/RoboDuino/tree/master/TobiSoft_1)	

# TobySoft 2.0

Arduino Mega based self driving car. Now with more crunchies.

## Sensors and Equipment
**L298N** - Fairly general purpose small motor controller

**SR04** (3x) - Front left, front middle, and front right array

**TOF10120** - Front facing IR time of flight distance sensor

**ADLX335** - Triple Axis Accelerometer *(actually not being utilized as of last verison of TobySoft 1.0)*

**SSD1306** - 128x64 OLED screen

**DFR0299** - DFPlayer Mini, a nifty little MP3 playing microcontroller *(and a small speaker for video game tunes)*

## Uno Pinout

|Pin| *sensor* | *sensor pin* | *notes* |
|--|--|--|--|
|***A0***|  |  |
|***A1***| Accelerometer | *X-Axis* |
|***A2***|  |  |
|***A3***|  |  |
|***A4*** *II2C*| OLED (SSD1306) | *SDA* |
|               | TOF10120 | *SDA* |
|***A5*** *II2C*| OLED (SSD1306) | *SCL* |
|               | TOF10120 | *SCL*|
|***D0*** *(TX)*| DFR0299 | *RX* | use of 1k resistor to reduce noise
|***D1*** *(RX)*| DFR0299 | *TX* |
|***D2***| Left SR04  | *Echo* | pin change interrupt used for echo timing
|***D3***| Center SR04 | *Echo* | pin change interrupt used for echo timing
|***D4***| Right SR04  | *Echo* | pin change interrupt used for echo timing
|***D5***| L298N |  |
|***D6***| L298N  |  |
|***D7***| L298N  |  |
|***D8***| L298N  |  |
|***D9***| L298N  |  |
|***D10***| SR04 Trigger | *Trig* | single trigger pin for all SR04s
|***D11***| L298N  |  |
|***D12***| IR Receiver | *Out* |
|***D13***| Onboard LED |  |









