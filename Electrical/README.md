# Electrical: The Stained Fluist
>
> Power the Glow

![Fluist in the Stained glass](/Assets/Images/Rendering.jpeg)

## Overview

Welcome to the electrifying side of the Stained Fluist! In the Electrical folder, we unravel the mysteries behind powering up our RGB spectacle and making it blink, glow, and maybe even dance a little.

## Features

- Power Control: The level of Voltages
- MCU: ESP32 our brain
- RGB: WS28111 DC 12V 36W
- ON/OFF: For those who like it simple
- CapTouch Control: You can't see it, but feel if
- OLED Display: But why?
- RESET: Just in case

### Power Control

The used RGB strips require 12V input. The Power Control circuit controls two states of power.
![Power Controll schemactic](/Electrical/PowerControl.png)
The Input is a AC230V DC12V power supply unit connected to the Frame.
The First High-Side Switch will control the power to the Buck-Converte used from the ESP32 without a HIGH signal the ESP32 will be in shutdown. The secound High-Side Switch will controll the Power to the RGB strips. This may be needed to reduce standby current.

### ESP32 WROOM

![ESP32WROOM Pinout](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2018/08/esp32-pinout-chip-ESP-WROOM-32.png?resize=1024%2C523&quality=100&strip=all&ssl=1)
<https://randomnerdtutorials.com/esp32-pinout-reference-gpios/>

| **Feature**       | **GPIO**     | **STATE**      |
|:------------------|:-------------|---------------:|
| **POWER CONTROL** |              |                |
| MCU POWER         |  GPIO12      | HIGH for power |
| RGB POWER         |  GPIO13      | HIGH for power |
|                   |              |                |
| **BUTTONS:**      |              |                |
| ON OFF            | GPIO27(39)   | active LOW     |
| CAP WHITE         | GPIO33(36)   | active HIGH    |
| CAP YELLOW        | GPIO32(34)   | active HIGH    |
| CAP ORANGE        | GPIO35       | active HIGH    |
| VIB MOTOR         | GPIO26       | HIGH for power |
|                   |              |                |
| **DISPLAY:**      |              |                |
| DISP SCL          | GPIO22       | PWM            |
| DISP SDA          | GPIO21       | PWM            |
|                   |              |                |
| **RGB Data:**     |              |                |
| RGB_0_M           | GPIO23       | PWM            |
| RGB_1_M           | GPIO25       | PWM            |
| RGB_2_M           | GPIO19       | PWM            |
| RGB_3_M           | GPIO18       | PWM            |
| RGB_4_M           | GPIO17       | PWM            |
| RGB_5_M           | GPIO16       | PWM            |
| RGB_6_M           | GPIOxx(27)   | PWM            |
| RGB_7_M           | GPIOxx(33)   | PWM            |
| RGB_8_M           | GPIOxx(32)   | PWM            |
| RGB_9_M           | GPIOxx(4)    | PWM            |
|                   |              |                |
| **IDEAS:**        |              |                |
| IR-RECIEVER       | GPIOxx()     |                |

### RGB

Model: [BTF-12V-30L-W](https://www.ebay.de/itm/222352737598?hash=item33c541853e:g:NmAAAOSwHORb4p8T)

Works with 3V3 Data in Signal

### ON OFF Button

Momentary button with Pull-Up to 3V3

<img src="https://m.media-amazon.com/images/I/61y5-XaNH1L._SL1500_.jpg" width="200" />

### Cap Sensors

Module: [TTP-223](https://www.epitran.it/ebayDrive/datasheet/23.pdf)

<img src="https://2.bp.blogspot.com/-0V_HsRNN3wQ/W1zV_KjT7JI/AAAAAAAABrw/YHZNMtmX_1cC7JYV1d77WNxgf3YeMewYQCLcBGAs/s1600/Touch-2.jpg" width="200" />

3 Buttons to control RGB brightness and color type (longpress) / effect (shot press)
