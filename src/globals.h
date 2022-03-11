#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "pin_define.h"

// arduino internal voltage reference enums
#define INTERNAL1V1 2
#define INTERNAL2V56 3

const uint8_t pin_count = 40;
const uint8_t mux_count = 5;

enum class PinType {
    MOSI,
    MISO,
    CLK,
    RESET,
    XTAL
};

struct Chip {
    uint8_t pin_count;
    pin_t MOSI;
    pin_t MISO;
    pin_t CLK;
    pin_t RESET;
    pin_t XTAL;
    pin_t GND[2];
    pin_t VCC[2];
    const char* name;
};

// EEPROM-related variables
const uint16_t EEPROM_calibration_offset_addr = 0;     // 40x2 bytes = 80 bytes of data
const uint16_t EEPROM_calibration_linear_addr = 80;     // 40x1 bytes = 40 bytes of data


#endif