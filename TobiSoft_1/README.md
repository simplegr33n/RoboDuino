[<img src="https://raw.githubusercontent.com/simplegr33n/RoboDuino/master/_assets/tobylogo2.bmp" width="600">](https://github.com/simplegr33n/RoboDuino/tree/master/TobiSoft_1)	

# TobySoft 1.0

Arduino Uno based self driving car. Documentation to allow myself a rebuild, as the project is being torn down and expanded to a Mega for TobySoft 2.0. While there are some pins remaining on the Uno, utilizing the DFPlayer requires use of the sole hardware Serial port which interferes with my ability to log.

Beyond having 5 hardware Serial ports to the Uno's 1, other advantages to the Mega include additional interrupt pins (though I got around that to this point by using pin change interrupts), additional flash memory and SRAM, 40 additional digital pins, 9 additional PWM enabled pins, and 10 additional analog pins. I doubt I will be able to utilize this board quite to the extent that I have with the Uno before needing additional processing power to achieve my goals (considering a Beagleboard or Jetson Nano as I try to implement computer vision).

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
|***D0*** *(TX)*| DF Mini MP3 Player | *RX* | use of 1k resistor to reduce noise
|***D1*** *(RX)*| DF Mini MP3 Player | *TX* |
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









