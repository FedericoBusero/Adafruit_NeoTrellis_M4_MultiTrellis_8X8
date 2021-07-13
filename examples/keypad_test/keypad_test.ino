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

//define a callback for key presses
TrellisCallback keypressed(keyEvent evt) {
  Serial.print((int)evt.bit.NUM);

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)   {
    Serial.println(" pressed");
  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    Serial.println(" released");
  }
  trellis.show();
  return 0;
}


void setup() {
  Serial.begin(115200);
  if (!trellis.begin()) {
    Serial.println("failed to begin trellis");
    while (1);
  }

  Serial.println("basic keypad test!");

  //activate rising and falling edges on all keys & register callback
  trellis.registerCallback(keypressed);

}

void loop() {
  trellis.read();
  delay(10);
}
