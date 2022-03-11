#ifndef INTERFACE_H
#define INTERFACE_H

#include "../globals.h"
#include "../chips.h"
#include "../hardware/multiplexer.h"
#include <ADCTouch.h>
#include <EEPROM.h>

namespace Interface {
    void setup();
    uint16_t read_voltage(pin_t pin, uint16_t samples = 128);
    uint16_t read_capacity_raw(pin_t pin, uint16_t samples = 128);
    uint16_t read_capacity(pin_t pin, uint16_t samples = 128);
    uint16_t* read_capacity_matrix(uint16_t samples, bool fast=false);
    uint16_t* read_capacity_raw_matrix(uint16_t samples, bool fast=false);
    uint16_t* read_voltage_matrix(uint16_t samples, pin_t pin);
    void apply_diode_voltage(pin_t pin);
    void clear_diode_voltage(pin_t pin);
    void clear_pins();
    void load_calibration();
};
#endif