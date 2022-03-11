#include "detector.h"

namespace Detector {

    uint8_t low_reset = 255;
    uint8_t chip_start = 255;
    uint8_t chip_end = 0;
    uint8_t chip_size = 0;
    bool rotated = false;
    Chip detected_chip = UnknownChip;


    void setup() {
        Interface::setup();
    }

    uint8_t chip_inserted(uint16_t* data) {
        chip_start = 255;
        chip_end = 0;
        low_reset = 255;
        rotated = false;
        detected_chip = UnknownChip;
        bool low_pin = false;
        bool result = true;
        for (int i=0; i<20; i++) {
            if (data[i]+data[39-i] < 50)
                continue;
            if (min(data[i], data[39-i]) < 50) {
                if (low_pin)
                    return 0;
                else
                    low_pin = true;
            } else if (chip_start != 255 && i - chip_end > 1)
                return 0;
            
            chip_start = min(chip_start, i);
            chip_end = max(chip_end, i);
        }
        chip_size = chip_end-chip_start+1;
        
        return (chip_size >= 4);
    }

    bool stable(uint16_t* data) {
        const uint8_t history = 5;
        const uint8_t relative_treshold = 25;
        const uint8_t absolute_treshold = 5;
        const uint16_t time_treshold = 10000;

        static uint16_t last_matrix[40*history] = {0};
        static uint32_t last_time[history] = {0};
        static uint8_t ptr = 0;

        bool response = true;

        last_time[ptr] = millis();
        for (int i=0; i<40; i++) {
            last_matrix[ptr*40 + i] = data[i];
        }
        ptr = (ptr+1) % 5;

        if (millis()-last_time[ptr] > time_treshold)
            return false;

        for (int pin=0; pin<40 && response; pin++) {
            uint16_t minimum = 1024;
            uint16_t maximum = 1;   // 1 to prevent a division by zero
            for (int r=0; r<history; r++) {
                minimum = min(minimum, last_matrix[r*40 + pin]);
                maximum = max(maximum, last_matrix[r*40 + pin]);
            }
            uint32_t dif = maximum - minimum;
            response &= ((dif*100)/maximum <= relative_treshold) || (dif <= absolute_treshold);
        }

        return response;
    }

    bool is_detected() {
        uint16_t* data = Interface::read_capacity_matrix(16, true);
        bool is_chip_inserted = chip_inserted(data);
        bool is_stable = stable(data);

        return is_chip_inserted && is_stable;
    }

    uint16_t* detect_special_pins() {
        uint16_t* data = Interface::read_capacity_matrix(32);
        uint16_t sorted[40] = {};
        uint32_t average = 0;
        uint8_t count = 0;
        for (int i=0; i<40; i++) {
            if (data[i] > 50) {
                Utils::sort_add(sorted, data[i]);
                average += data[i];
                count++;
            } else if (data[39-i] > 50) {
                low_reset = i;
            }
        }
        average /= count;
        // only the top 5 pins are relevant as power and reset
        static uint16_t tops[5] = {0}; // lower byte indicates pin and higher byte is propability
        for (int t=0; t<5; t++) {
            uint8_t propability;
            uint8_t position;
            if (sorted[t] <= average || sorted[t] <= 25) {
                propability = 0;
            } else {
                propability = (100 * (sorted[t] - average)) / (sorted[0] - average);
            }
            for (int i=0; i<40; i++) {
                if (data[i] == sorted[t]) {
                    position = i;
                    break;
                }
            }
            tops[t] = (propability << 8) + position;
        }
        return tops;
    }

    bool is_ground(pin_t pin) {
        Interface::apply_diode_voltage(pin);
        uint16_t* data = Interface::read_voltage_matrix(64, pin);
        Interface::clear_diode_voltage(pin);
        uint8_t count = 0;
        for (int i=0; i<40; i++) {
            if (data[i] > 100)
                count++;
        }
        return (count >= chip_size - 2 - chip_size/10);
    }

    uint8_t chip_pin(pin_t pin, bool rotate=false) {   // returns pin numbered relatively, to compare with chip numbering
        uint8_t num;
        if (pin < 20) { // right side
            num = (pin - chip_start);
        } else { // left side
            num = (39-pin) - chip_start + chip_size;
        }
        return rotate ? chip_size*2-1 - num : num;
    }

    uint16_t calculate_matching(Chip chip, uint8_t* pins, uint8_t* propabilities, bool* grounds, bool rot=false) {
        if (chip_size*2 != chip.pin_count)
            return 0;
        
        uint16_t propability = 0;
        if (low_reset != 255 && chip.RESET == chip_pin(low_reset, rot)) {
            propability += 100;
        } else if (low_reset == 255) {
            for (int i=0; i<5; i++) {
                if (!grounds[i] && chip.RESET == chip_pin(pins[i], rot))
                    propability += propabilities[i];
            }
        }

        for (int i=0; i<5; i++) {
            if (grounds[i] && (chip.GND[0] == chip_pin(pins[i], rot) || chip.GND[1] == chip_pin(pins[i], rot))) {
                propability += propabilities[i];
            } else if (!grounds[i] && (chip.VCC[0] == chip_pin(pins[i], rot) || chip.VCC[1] == chip_pin(pins[i], rot))) {
                propability += propabilities[i];
            }
        }
        return propability;
    }

    void identify_chip() {
        Chip propable_chip = UnknownChip;
        uint16_t chip_propability = 0;
        uint16_t* special_pins = detect_special_pins();
        uint8_t pins[5] = {0};
        uint8_t propabilities[5]= {0};
        bool grounds[5] = {0};
        for (int i=0; i<5; i++) {
            pins[i] = special_pins[i] & 255;
            propabilities[i] = (special_pins[i] & (255 << 8)) >> 8;
            
            if (propabilities[i] == 0)
                continue;
            grounds[i] = is_ground(pins[i]);
        }

        for (Chip chip : chips) {
            uint16_t propability = calculate_matching(chip, pins, propabilities, grounds);
            if (propability > chip_propability) {
                propable_chip = chip;
                chip_propability = propability;
            }
        }        
        for (Chip chip : chips) {
            uint16_t propability = calculate_matching(chip, pins, propabilities, grounds, true);
            if (propability > chip_propability) {
                rotated = true;
                propable_chip = chip;
                chip_propability = propability;
            }
        }
        detected_chip = propable_chip;
    }

    pin_t get_pin_location(uint8_t pin) {
        if (rotated) 
            pin = chip_size*2-1 - pin;
        if (pin < chip_size) {
            return pin + chip_start;
        } else {
            return 39 - (pin-chip_size) - chip_start;
        }
    }
    
    void power_up() {
        Interface::clear_pins();
        digitalWrite(MUX_INHIBIT_CTR, LOW);
        for (uint8_t pin_gnd : detected_chip.GND) {
            if (pin_gnd != 255) {
                pin_t pin_loc = get_pin_location(pin_gnd);
                pinMode(IO[pin_loc], OUTPUT);
                digitalWrite(IO[pin_loc], LOW);
            }
        }
        for (uint8_t pin_vcc : detected_chip.VCC) {
            if (pin_vcc != 255) {
                pin_t pin_loc = get_pin_location(pin_vcc);
                pinMode(IO[pin_loc], OUTPUT);
                digitalWrite(IO[pin_loc], HIGH);
            }
        }
    }

    void power_down() { 
        for (uint8_t pin_vcc : detected_chip.VCC) {
            if (pin_vcc != 255) {
                pinMode(IO[get_pin_location(pin_vcc)], INPUT);
            }
        }
        for (uint8_t pin_gnd : detected_chip.GND) {
            if (pin_gnd != 255) {
                pinMode(IO[get_pin_location(pin_gnd)], INPUT);
            }
        }
        Interface::clear_pins();
        digitalWrite(MUX_INHIBIT_CTR, HIGH);
    }

    pin_t get_pin(PinType pin_type) {
        if (pin_type == PinType::CLK) {
            return get_pin_location(detected_chip.CLK);
        } else if (pin_type == PinType::MISO) {
            return get_pin_location(detected_chip.MISO);
        } else if (pin_type == PinType::MOSI) {
            return get_pin_location(detected_chip.MOSI);
        } else if (pin_type == PinType::RESET) {
            return get_pin_location(detected_chip.RESET);
        } else if (pin_type == PinType::XTAL) {
            return get_pin_location(detected_chip.XTAL);
        }
    }

}