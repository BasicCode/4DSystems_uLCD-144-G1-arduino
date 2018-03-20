# 4D Systems uLCD-144 G1

This is a library for the 4D Systems uLCD-144 G1 (SGC) which is a 128 x 128 px colour LCD screen and driver by 4D Systems. NOTE: that this is for the discontinued G1 version of the device.
I wrote this library because I could not find a functioning Arduino library for the device on the vendor website or elsewhere online. The name Goldilox used for the class refers to the
on-bard LCD driver that 4D Systems developed.

Product page: <br>
http://old.4dsystems.com.au/prod.php?id=256

## Installation
Download the ZIP file and extract it to the **Arduino/libraries** folder. Open the Arduino IDE, click **File**-**Examples**-**Goldilox_uLCD_test**.

| LCD Breakout Pin | Arduino Pin |
| :--------- | :---------- |
| 9 (Vcc) | 5V Out |
| 8 (TX) | 0 (UART RX) |
| 7 (RX) | 1 (UART TX) |
| 6 (GND) | GND |
| 4 (RST) | 2 (default) |

## Usage
The library supports most of the functions of the Goldilox uLCD driver (Well all of the useful ones anyway). Communication must begin with an initialisation routine, power-on-reset, and baud detect.
The **initLCD()** accepts the address of the HardwareSerial port, the desired baud rate, and the reset pin being used. This routine handles all of this and waits for the LCD to acknowledge. 
The example includes a 1000ms delay before running the initialisation routine. While this shouldn't strictly be necessary I have found that it can cause unpredictable behaviour if initialising too early.
'''
Goldilox_uLCD uLCD;

uLCD.initLCD(&Serial1, 9600, reset_pin);
'''
This routine includes a short delay after power-on-reset while the device stabilises and then you are ready to start sending commands.

## Notes
* I have found this device to be quite fickle with timing. Some devices seem to run it fine at 115200 baud, while others need to go down to 9600. You might just have to play with this number and see what works for you.<br>
* If there is a problem with one command then it will typically cause problems with subsequent commands by jumbling up the device command registers. Another reason to play with the delays and UART timing.<br>
* The device is visibly slow and there is no way around it. I have found the best way to update the screen is to draw a black (or background-coloured) box only over the part of screen that you want to refresh
rather than re-draw the whole screen. **BGColour()** and **clearLCD()** are very slow operations and best avoided.