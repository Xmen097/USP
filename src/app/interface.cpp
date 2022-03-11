#include "interface.h"

namespace Interface {

    uint16_t offset_calibration[40] = {0};
    uint8_t linear_calibration[40] = {0};

    uint16_t read_capacity_raw(pin_t pin, uint16_t samples) {
        analogReference(DEFAULT);
        multiplexer.mode_ADC(pin);
        pin_t pinADC = multiplexer.set_pin(pin);
        digitalWrite(MUX_INHIBIT_CTR, LOW);
        pinMode(pinADC, INPUT);
        delayMicroseconds(100);	

        uint32_t value = 0; // Begins the timer
        digitalWrite(LEDB, HIGH);
        for (int i=0; i<samples; i++) {
            pinMode(IO[pin], INPUT_PULLUP); // Begins charging the capacitor
            pinMode(pinADC, OUTPUT);
            digitalWrite(pinADC, LOW);
            delayMicroseconds(5);
            pinMode(pinADC, INPUT);
            pinMode(IO[pin], INPUT);
            value += analogRead(pinADC);
        }

        digitalWrite(LEDB, LOW);
        pinMode(pinADC, INPUT);
        pinMode(IO[pin], INPUT);	
        
        delayMicroseconds(100);

        return value/samples;
    }
    

    uint16_t read_capacity(pin_t pin, uint16_t samples) {
        uint16_t capacity = read_capacity_raw(pin, samples);
        if (offset_calibration[pin] <= capacity) {
            return 0;
        } else {
            return (((uint32_t)(offset_calibration[pin] - capacity)) * (uint32_t)linear_calibration[pin]) / 128;
        }
    }

    uint16_t* read_matrix_precise(uint16_t samples, uint16_t (*func)(pin_t, uint16_t)) {
        static uint16_t matrix[40] = {0};
        uint32_t sum[40] = {0};
        uint16_t counts[40] = {0};

        for (uint16_t i=0; i<40*samples; i++) {
            pin_t pos = random(40);
            sum[pos] += func(pos, 16);
            counts[pos] += 1;
        }
        for (uint8_t i=0; i<40; i++)
            matrix[i] = sum[i]/counts[i];
        
        return matrix;
    }
    uint16_t* read_matrix_fast(uint16_t samples, uint16_t (*func)(pin_t, uint16_t)) {
        static uint16_t matrix[40] = {0};

        for (uint16_t i=0; i<40; i++) {
            matrix[i] = func(i, samples);
        }        
        for (uint16_t i=0; i<40; i++) {
            matrix[39-i] = (matrix[39-i] + func(39-i, samples)) / 2;
        }
        
        return matrix;
    }

    uint16_t* read_capacity_matrix(uint16_t samples, bool fast=false) {
        if (fast)
            return read_matrix_fast(samples, read_capacity);
        else
            return read_matrix_precise(samples, read_capacity);
    }
    uint16_t* read_capacity_raw_matrix(uint16_t samples, bool fast=false) {
        if (fast)
            return read_matrix_fast(samples, read_capacity_raw);
        else
            return read_matrix_precise(samples, read_capacity_raw);
    }

    uint16_t read_voltage(pin_t pin, uint16_t samples) {
        analogReference(INTERNAL1V1);
        pinMode(IO[pin], INPUT);
        multiplexer.mode_ADC(pin);
        pin_t pinADC = multiplexer.set_pin(pin);
        pinMode(pinADC, INPUT);
        uint32_t sum = 0;
        delayMicroseconds(100);
        for (int i=0; i<samples; i++) {
            sum += analogRead(pinADC);
        }
        return sum / samples;
    }

    uint16_t* read_voltage_matrix(uint16_t samples, pin_t pin) {
        static uint16_t matrix[40] = {0};
        for (int i=0; i<40; i++)
            matrix[i] = 0;

        if (pin < 38 && pin > 21) { // pin is on the left side
            for (int i=0; i<24; i++) {
                matrix[i] = read_voltage((38+i) % 40, samples);
            }
        } else {    // pin is on right side
            for (int i=22; i<38; i++) {
                matrix[i] = read_voltage(i, samples);
            }
        }
        return matrix;
    }

    void apply_diode_voltage(pin_t pin) {
        pinMode(IO[pin], INPUT_PULLUP);
        multiplexer.mode_ADC(pin);
        pin_t adc = multiplexer.set_pin(pin);
        pinMode(adc, OUTPUT);
        digitalWrite(adc, LOW);
        digitalWrite(MUX_INHIBIT_CTR, LOW);
        delayMicroseconds(100);
    }

    void clear_diode_voltage(pin_t pin) {
        pin_t adc = multiplexer.get_pin(pin);
        pinMode(IO[pin], INPUT);
        pinMode(adc, INPUT);
        digitalWrite(MUX_INHIBIT_CTR, HIGH);
        delayMicroseconds(100);
    }

    void clear_pin(pin_t pin) {
        pinMode(IO[pin], INPUT);
    }

    void clear_pins() {
        for (pin_t pin=0; pin<40; pin++) {
            clear_pin(pin);
        }
    }

    void load_calibration() {
        EEPROM.get(EEPROM_calibration_offset_addr, offset_calibration);
        EEPROM.get(EEPROM_calibration_linear_addr, linear_calibration);
    }

    void setup() {
        clear_pins();
        multiplexer.setup();
        load_calibration();
    }
}