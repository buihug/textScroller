# textScroller
*Text scroller for the Arduino Uno provided with a LiquidCrystal_I2C lcd display*

.

**Needed**
- Arduino Uno
- [Liquid Crystal display with the I2C connector](https://create.arduino.cc/projecthub/arduino_uno_guy/i2c-liquid-crystal-displays-5b806c)
- PC running a Serial Monitor (9600 bouds)
- USB cable to power the Arduino and transmit Serial Monitor data<br>

.

**Hint**

The user can inject a new phrase via Serial Monitor:
   - Simply type a new phrase and hit `Enter`

.

**How it works**
- At start up, the word 'Hello' scrolls from right to left
- When Serial Monitor detects keyboard activity, anything typed followed by `Enter` becomes the new phrase (no confirmation asked)
- Then, the scroller parses a short spacer ('....') and the new phrase
- Additional keyboard activity interrupts the current phrase with the spacer and sets to scroll whatever was typed before `Enter`
- If `Enter` is not recieved within 10 seconds of keyboard activity, text injection aborts and scrolling resumes as with the old phrase

.

![example of phrase injection via Serial Monitor activity](Serial%20Monitor%20for%20textScroller.jpg)

.

.

Serial Monitor activity is mediated by *SerialCommander*,

a library created by Cristian D. Szwarc

[https://github.com/cristianszwarc/SerialCommander](https://github.com/cristianszwarc/SerialCommander)
   
   
