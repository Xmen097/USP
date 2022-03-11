#ifndef DISPLAY_H
#define DISPLAY_H

#include "../globals.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display_resources.h"

namespace Display {
    void setup();
    void debug_info(uint16_t *data);
    void write(const __FlashStringHelper * text);
    void write(const char* text);
    void write(const uint16_t number);
    void clear();
}

#endif