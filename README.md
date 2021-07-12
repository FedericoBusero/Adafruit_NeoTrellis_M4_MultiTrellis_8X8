# Adafruit_NeoTrellis_M4_MultiTrellis_8X8
Arduino library for the Adafruit Neotrellis M4 Mainboard with 2 trellis boards (multitrellis) in a 8x8 matrix. Following hardware is necessary:
- 1x Adafruit 3938 Neotrellis M4 Mainboard
- 2x Adafruit 3954 Neotrellis 4x4 RGB keypad
- 4x Adafruit 1611 Silicone Elastomer 4x4 Button keypad

# Soldering and case
When soldering, first have a look at the pin order of the connector: the left side has an opposite order of the right side!
You can see the pinout at https://cdn-learn.adafruit.com/assets/assets/000/064/887/original/adafruit_products_fab.png?1541024062

Have look at https://www.thingiverse.com/thing:4882667 to see an example of a lasercut case.

# Software library dependencies
- Adafruit Keypad https://github.com/adafruit/Adafruit_Keypad
- Adafruit NeoPixel https://github.com/adafruit/Adafruit_NeoPixel
- Adafruit DMA neopixel library https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA
- Adafruit Seesaw Library https://github.com/adafruit/Adafruit_Seesaw
- Adafruit BusIO library https://github.com/adafruit/Adafruit_BusIO
