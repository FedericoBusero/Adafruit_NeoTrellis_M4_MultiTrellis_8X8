/*!
   @file Adafruit_M4MultiTrellis_8x8.cpp

   @mainpage Adafruit NeoTrellis M4 MultiTrellis 8X8 Library

   @section intro_sec Introduction

   Arduino library for the Adafruit Neotrellis M4 Mainboard with 2 trellis boards (multitrellis) in a 8x8 matrix

   @section author Author

   Written FedericoBusero
   Based on code from the Adafruit MultiTrellis library and NeoTrellisM4 library by Limor Fried/Ladyada 

*/

#include "Adafruit_M4MultiTrellis_8x8.h"

static const byte NEO_PIN = 10;

static const byte M4ROWS = 4; // four rows
static const byte COLS = 8; // eight columns

// define the symbols on the buttons of the keypads
static const byte trellisKeys[M4ROWS][COLS] = {{0, 1, 2, 3, 4, 5, 6, 7},
  {8, 9, 10, 11, 12, 13, 14, 15},
  {16, 17, 18, 19, 20, 21, 22, 23},
  {24, 25, 26, 27, 28, 29, 30, 31}
};

static byte rowPins[M4ROWS] = {14, 15, 16, 17}; // connect to the row pinouts of the keypad
static byte colPins[COLS] = {2, 3, 4, 5, 6, 7, 8, 9}; // connect to the column pinouts of the keypad

static TrellisCallback (*s_cb)(keyEvent) = NULL;

#define MULTITRELLIS_Y_DIM 4 //number of rows of key on multitrellis part
#define MULTITRELLIS_X_DIM 8 //number of columns of keys on multitrellis part

static TrellisCallback onMultiTrellisEvent(keyEvent evt) {
  evt.bit.NUM += M4ROWS * COLS;
  if (s_cb)
  {
    (*s_cb)(evt);
  }
  return 0;
}

/**************************************************************************/
/*!
    @brief  class constructor
    @param  trelli pointer to a array of 2 neotrellis objects.
            these object must have their I2C addresses specified in the class
            constructors.
*/
/**************************************************************************/
Adafruit_M4MultiTrellis_8x8::Adafruit_M4MultiTrellis_8x8(Adafruit_NeoTrellis *trellis_array)
  : keypad(makeKeymap(trellisKeys), rowPins, colPins, M4ROWS, COLS),
    ledstrip(M4ROWS * COLS, NEO_PIN, NEO_GRB),
    multitrellis(trellis_array, MULTITRELLIS_Y_DIM / 4, MULTITRELLIS_X_DIM / 4) {
}

/**************************************************************************/
/*!
    @brief  Initialize the NeoTrellis, begin communication with the matrix 
    of neotrellis boards, start the keypad scanner and turn all neopixels off.

    @returns true on success, false otherwise.
*/
/**************************************************************************/
bool Adafruit_M4MultiTrellis_8x8::begin(void) {
  keypad.begin();

  // Initialize all pixels to 'off'
  ledstrip.begin();
  for (int i = 0; i < M4ROWS * COLS; i++) {
    ledstrip.setPixelColor(i, 0x00);
  }
  ledstrip.show();
  ledstrip.setBrightness(255);

  return multitrellis.begin();
}


/**************************************************************************/
/*!
    @brief  set the color of a neopixel at a key number.
    @param  num the keynumber to set the color of. Key 0 is in the top left
            corner of the trellis matrix, key 1 is directly to the right of it,
            and the last key number is in the bottom righthand corner.
    @param  color the color to set the pixel to. This is a 24 bit RGB value.
            for example, full brightness red would be 0xFF0000, and full
            brightness blue would be 0x0000FF.
*/
/**************************************************************************/
void Adafruit_M4MultiTrellis_8x8::setPixelColor(uint32_t num, uint32_t color) {
  if (pixel < M4ROWS * COLS) {
    ledstrip.setPixelColor(pixel, color);
  }
  else {
    multitrellis.setPixelColor(pixel % COLS, (pixel / COLS) - 4, color);
  }
}

/**************************************************************************/
/*!
    @brief  set the color of a neopixel at a key index.
    @param  x the column index of the key. column 0 is on the lefthand side of
            the matix.
    @param  y the row index of the key. row 0 is at the top of the matrix and
            the numbers increase downwards.
    @param  color the color to set the pixel to. This is a 24 bit RGB value.
            for example, full brightness red would be 0xFF0000, and full
            brightness blue would be 0x0000FF.
*/
/**************************************************************************/
void Adafruit_M4MultiTrellis_8x8::setPixelColor(int x, int y,  uint32_t color ) {
  if (y < 4)
  {
    ledstrip.setPixelColor(y * 8 + x, color);
  }
  else
  {
    multitrellis.setPixelColor(x, y - 4, color);
  }
}

/**************************************************************************/
/*!
    @brief  Updates all neopixels both on M4 mainboard and multitrellis
*/
/**************************************************************************/
void Adafruit_M4MultiTrellis_8x8::show() {
  ledstrip.show();
  multitrellis.show();
}

/**************************************************************************/
/*!
    @brief  Activate all keys and register a callback function for key events
*/
/**************************************************************************/
void Adafruit_M4MultiTrellis_8x8::registerCallback(TrellisCallback (*cb)(keyEvent)) {
  s_cb = cb;
  for (int y = 0; y < MULTITRELLIS_Y_DIM; y++) {
    for (int x = 0; x < MULTITRELLIS_X_DIM; x++) {
      //activate rising and falling edges on all keys
      multitrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      multitrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
      multitrellis.registerCallback(x, y, onMultiTrellisEvent);
    }
  }
}

/**************************************************************************/
/*!
    @brief  read all key events (both of M4 mainboard as of trellis boards) 
    and call the callback function
*/
/**************************************************************************/
void Adafruit_M4MultiTrellis_8x8::read(void) {
  keypad.tick();
  // look for an entire column being pressed at once and if it was, clear the
  // whole buffer
  uint8_t rcount[] = {0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < (COLS * M4ROWS) - 1; i++) {
    if (keypad.justPressed(i + 1, false))
      rcount[i % COLS]++;
  }
  for (int i = 0; i < COLS; i++) {
    if (rcount[i] >= M4ROWS) {
      keypad.clear();
      break;
    }
  }

  while (keypad.available()) {
    keypadEvent e = keypad.read();
    keyEvent evt;

    evt.bit.NUM = e.bit.KEY;
    if (e.bit.EVENT == KEY_JUST_PRESSED)
    {
      evt.bit.EDGE = SEESAW_KEYPAD_EDGE_RISING;
      if (s_cb)
      {
        (*s_cb)(evt);
      }
    }
    else if (e.bit.EVENT == KEY_JUST_RELEASED)
    {
      evt.bit.EDGE = SEESAW_KEYPAD_EDGE_FALLING;
      if (s_cb)
      {
        (*s_cb)(evt);
      }
    }
  }

  multitrellis.read();
}

/**************************************************************************/
/*!
    @brief  returns the number of keys
*/
/**************************************************************************/
uint8_t Adafruit_M4MultiTrellis_8x8::num_keys(void) {
  return (MULTITRELLIS_X_DIM * (M4ROWS + MULTITRELLIS_Y_DIM));
}
