#include "debug.h"

namespace Debug {

    const uint16_t calibration_addr = 42;
    
    void voltage_readout() {
        uint16_t data[40] = {0}; 
        for (int i=22; i<38; i++) {
            data[i] = Interface::read_voltage(i);
        }
        Display::debug_info(data);
    }

    void capacity_readout() {
        uint16_t* data = Interface::read_capacity_matrix(16);
        Display::debug_info(data);
    }

    void capacity_raw_readout() {
        uint16_t data[40] = {0}; 
        for (int i=0; i<40; i++) {
            data[i] = Interface::read_capacity_raw(i)-350;
        }
        Display::debug_info(data);
    }

    void calibrate_capacity_offset() {
        uint16_t calibration_matrix[40] = {0};
        uint16_t* ref = Interface::read_capacity_raw_matrix(32);
        for (int i=0; i<40; i++) {
            calibration_matrix[i] = ref[i];
        }
        EEPROM.put(EEPROM_calibration_offset_addr, calibration_matrix);
    }

    void calibrate_capacity_linear() {
        uint8_t calibration_matrix[40];
        for (int i=0; i<40; i++) 
            calibration_matrix[i]=128;
        EEPROM.put(EEPROM_calibration_linear_addr, calibration_matrix);
        Interface::load_calibration();
        for (pin_t pin=0; pin<40; pin++) {
            while(true) {
                Input::loop();
                if (Input::just_released(Button::OK)) {
                    break;
                } else if (Input::just_released(Button::BACK)) {
                    pin--;
                    break;
                }
            }
            uint16_t* data = Interface::read_capacity_matrix(64); 
            calibration_matrix[pin] = ((uint32_t)200*(uint32_t)128)/data[pin]; // calculate a linear scaling factor to scale value to 200. Internally saved as multiple of 128
            Display::write(calibration_matrix[pin]*100+pin);
        }
        EEPROM.put(EEPROM_calibration_linear_addr, calibration_matrix);
    }

    void calibrate_to_eeprom() {
        Display::write(F("Calibration started"));
        calibrate_capacity_offset();
        Display::write(F("Calibrating linear"));
        calibrate_capacity_linear();
    }
}