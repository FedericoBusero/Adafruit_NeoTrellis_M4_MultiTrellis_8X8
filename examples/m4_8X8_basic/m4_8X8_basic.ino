/* This example shows basic usage of the M4 NeoTrellis mainboard 
  connected to 2 NeoTrellis boards (MultiTrellis) in a 8x8 matrix

  As is this example shows use of two NeoTrellis boards
  connected together with the leftmost board having the
  default I2C address of 0x2E, and the rightmost board
  having the address of 0x2F (the A0 jumper is soldered)
*/

#include "Adafruit_M4MultiTrellis_8x8.h"

#define X_DIM 8 //number of columns of keys
#define Y_DIM 8 //number of rows of keys (M4 mainboard and multitrellis included)

//create a matrix of trellis panels
Adafruit_NeoTrellis t_array[(Y_DIM-4)/4][X_DIM / 4] = {
  { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F) }
};

//pass this matrix to the multitrellis object
Adafruit_M4MultiTrellis_8x8 trellis((Adafruit_NeoTrellis *)t_array);

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}

//define a callback for key presses
TrellisCallback blink(keyEvent evt) {

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    trellis.setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, X_DIM * Y_DIM, 0, 255))); //on rising
  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    trellis.setPixelColor(evt.bit.NUM, 0); //off falling
  }

  trellis.show();
  return 0;
}

void setup() {
  Serial.begin(115200);
  //while(!Serial);

  if (!trellis.begin()) {
    Serial.println("failed to begin trellis");
    while (1);
  }

  /* the array can be addressed as x,y or with the key number */
  for (int i = 0; i < Y_DIM * X_DIM; i++) {
    trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM * Y_DIM, 0, 255))); //addressed with keynum
    trellis.show();
    delay(50);
  }

  for (int y = 0; y < Y_DIM; y++) {
    for (int x = 0; x < X_DIM; x++) {
      trellis.setPixelColor(x, y, 0x000000); //addressed with x,y
      trellis.show(); //show all LEDs
      delay(50);
    }
  }
  
  //activate rising and falling edges on all keys & register callback
  trellis.registerCallback(blink);
}

void loop() {
  trellis.read();
  delay(20);
}
