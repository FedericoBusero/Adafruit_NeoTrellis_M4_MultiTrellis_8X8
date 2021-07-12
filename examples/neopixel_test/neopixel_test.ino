#include "Adafruit_M4MultiTrellis_8x8.h"

/* This example shows basic usage of the M4 NeoTrellis mainboard 
  connected to 2 Trellis boards (MultiTrellis) in a 8x8 matrix

  As is this example shows use of two NeoTrellis boards
  connected together with the leftmost board having the
  default I2C address of 0x2E, and the rightmost board
  having the address of 0x2F (the A0 jumper is soldered)
*/

//create a matrix of trellis panels
Adafruit_NeoTrellis t_array[] = { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F)};

//pass this matrix to the Adafruit_M4MultiTrellis_8x8 object
Adafruit_M4MultiTrellis_8x8 trellis((Adafruit_NeoTrellis *)t_array);


void setup() {
  if (!trellis.begin()) {
    Serial.println("failed to begin trellis");
    while (1);
  }

  trellis.begin();
  trellis.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(seesaw_NeoPixel::Color(255, 0, 0), 50); // Red
  colorWipe(seesaw_NeoPixel::Color(0, 255, 0), 50); // Green
  colorWipe(seesaw_NeoPixel::Color(0, 0, 255), 50); // Blue
  //colorWipe(seesaw_NeoPixel::Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  theaterChase(seesaw_NeoPixel::Color(127, 127, 127), 50); // White
  theaterChase(seesaw_NeoPixel::Color(127, 0, 0), 50); // Red
  theaterChase(seesaw_NeoPixel::Color(0, 0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < trellis.num_keys(); i++) {
    trellis.setPixelColor(i, c);
    trellis.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < trellis.num_keys(); i++) {
      trellis.setPixelColor(i, Wheel((i + j) & 255));
    }
    trellis.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < trellis.num_keys(); i++) {
      trellis.setPixelColor(i, Wheel(((i * 256 / trellis.num_keys()) + j) & 255));
    }
    trellis.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < trellis.num_keys(); i = i + 3) {
        trellis.setPixelColor(i + q, c);  //turn every third pixel on
      }
      trellis.show();

      delay(wait);

      for (uint16_t i = 0; i < trellis.num_keys(); i = i + 3) {
        trellis.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < trellis.num_keys(); i = i + 3) {
        trellis.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      trellis.show();

      delay(wait);

      for (uint16_t i = 0; i < trellis.num_keys(); i = i + 3) {
        trellis.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
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
