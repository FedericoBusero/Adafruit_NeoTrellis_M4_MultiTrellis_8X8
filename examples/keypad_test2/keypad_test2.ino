#include "Adafruit_M4MultiTrellis_8x8.h"

/* This example shows basic usage of the M4 NeoTrellis mainboard 
  connected to 2 NeoTrellis boards (MultiTrellis) in a 8x8 matrix

  As is this example shows use of two NeoTrellis boards
  connected together with the leftmost board having the
  default I2C address of 0x2E, and the rightmost board
  having the address of 0x2F (the A0 jumper is soldered)
*/

//create a matrix of trellis panels
Adafruit_NeoTrellis t_array[] = { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F)};

//pass this matrix to the Adafruit_M4MultiTrellis_8x8 object
Adafruit_M4MultiTrellis_8x8 trellis((Adafruit_NeoTrellis *)t_array);

boolean *lit_keys;

//define a callback for key presses
TrellisCallback keypressed(keyEvent evt) {
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)   {
    int key = evt.bit.NUM;  // shorthand for what was pressed
    Serial.print(key); Serial.println(" pressed");
    lit_keys[key] = !lit_keys[key];
    if (lit_keys[key]) {
      trellis.setPixelColor(key, Wheel(random(255)));
    } else {
      trellis.setPixelColor(key, 0);
    }
  }
  trellis.show();
  return 0;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup() {
  Serial.begin(115200);

  if (!trellis.begin()) {
    Serial.println("failed to begin trellis");
    while (1);
  }

  //activate rising and falling edges on all keys & register callback
  trellis.registerCallback(keypressed);

  Serial.println("toggle keypad test!");

  lit_keys = new boolean[trellis.num_keys()];

  for (int i = 0; i < trellis.num_keys(); i++) {
    lit_keys[i] = false;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  trellis.read();
  delay(10);
}
