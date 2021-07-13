/*!
   @file Adafruit_M4MultiTrellis_8x8.h
*/

#ifndef _ADAFRUIT_M4MULTITRELLIS_8X8_H_
#define _ADAFRUIT_M4MULTITRELLIS_8X8_H_

#include <Adafruit_Keypad.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>
#include <Adafruit_NeoTrellis.h>

#include <Arduino.h>

#ifndef ADAFRUIT_TRELLIS_M4_EXPRESS
#error "This library is only for the Adafruit NeoTrellis M4!!"
#endif

/*!    @brief  Adafruit NeoTrellis M4 Multitrellis 8x8 library that handles DMA NeoPixel, Keypad
   scanning and control of 2 NeoTrellis boards (multitrellis) */
class Adafruit_M4MultiTrellis_8x8 {

  public:
    Adafruit_M4MultiTrellis_8x8(Adafruit_NeoTrellis *trellis_array);
    bool begin(void);
    void read(void);
    void show();

    void setPixelColor(int x, int y,  uint32_t color );
    void setPixelColor(uint32_t num, uint32_t color);

    void registerCallback(TrellisCallback (*cb)(keyEvent));
    uint8_t num_keys(void);
    
  private:
    Adafruit_MultiTrellis multitrellis;
    Adafruit_Keypad keypad;
    Adafruit_NeoPixel_ZeroDMA ledstrip;
};

#endif
