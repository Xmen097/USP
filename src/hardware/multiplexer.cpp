#include "multiplexer.h"

class Mux : public Multiplexer {
  private:
    const pin_t pinA;
    const pin_t pinB;
    const pin_t pinC;
    const pin_t pinADC;
    const pin_t pinCTR;
    const uint8_t *mapping;
    bool is_mode_adc = true;

  public:
    Mux(pin_t _pinA, pin_t _pinB, pin_t _pinC, pin_t _pinADC, pin_t _pinCTR, const uint8_t _mapping[8])
        : pinA(_pinA), pinB(_pinB), pinC(_pinC), pinADC(_pinADC), pinCTR(_pinCTR), mapping(_mapping)
    {}

    void mode_ADC(uint8_t id) override {
        if (is_mode_adc) return;
        is_mode_adc = true;
        digitalWrite(pinCTR, LOW);
        delayMicroseconds(10);
        pinMode(pinADC, INPUT);
        delayMicroseconds(10);
    }
    
    void mode_VCC(uint8_t id) override {
        if (!is_mode_adc) return;
        is_mode_adc = false;
        pinMode(pinADC, OUTPUT);
        digitalWrite(pinADC, HIGH);
        delayMicroseconds(10);
        digitalWrite(pinCTR, HIGH);
        delayMicroseconds(10);
    }
    
    int8_t pin_disabled(uint8_t id) override {
        uint8_t state = 0;
        state |= !digitalRead(pinA) << 0;
        state |= !digitalRead(pinB) << 1;
        state |= !digitalRead(pinC) << 2;
        if (id == state) {
            return pinADC;
        } else {
            return -1;
        }
    }

    pin_t get_pin(uint8_t id) override {
        return pinADC;
    }

    pin_t set_pin(uint8_t id) override {
        id = mapping[id];
        digitalWrite(pinA, (~id) & 0b001);
        digitalWrite(pinB, (~id) & 0b010);
        digitalWrite(pinC, (~id) & 0b100);
        delayMicroseconds(10); // wait for the mux to stabilize - datasheet states it could take up to hundreds of nanoseconds
        return pinADC;
    }

    void setup() {
        pinMode(pinA, OUTPUT);
        digitalWrite(pinA, LOW);
        pinMode(pinB, OUTPUT);
        digitalWrite(pinB, LOW);
        pinMode(pinC, OUTPUT);
        digitalWrite(pinC, LOW);
        pinMode(pinCTR, OUTPUT);
        digitalWrite(pinCTR, LOW);
        pinMode(pinADC, INPUT);
    }
};


const uint8_t mapping1[8] = {4, 6, 3, 5, 7, 0, 1, 2};
const uint8_t mapping2[8] = {5, 4, 6, 7, 3, 0, 1, 2};
const uint8_t mapping3[8] = {6, 7, 5, 4, 3, 0, 2, 1};
const uint8_t mapping4[8] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint8_t mapping5[8] = {6, 4, 7, 5, 3, 0, 1, 2};

Mux* mux1 = new Mux(MUXL_A, MUXL_B, MUXL_C, MUX1_ADC, MUX1_CTR, mapping1);
Mux* mux2 = new Mux(MUXL_A, MUXL_B, MUXL_C, MUX2_ADC, MUX2_CTR, mapping2);
Mux* mux3 = new Mux(MUXL_A, MUXL_B, MUXL_C, MUX3_ADC, MUX3_CTR, mapping3);
Mux* mux4 = new Mux(MUXR_A, MUXR_B, MUXR_C, MUX4_ADC, MUX4_CTR, mapping4);
Mux* mux5 = new Mux(MUXR_A, MUXR_B, MUXR_C, MUX5_ADC, MUX5_CTR, mapping5);

Muxes::Muxes() {
    muxes[0] = mux1;
    muxes[1] = mux2;
    muxes[2] = mux3;
    muxes[3] = mux4;
    muxes[4] = mux5;
}

int mux_id(pin_t pin) {
    return (pin+2) & 0b111; // Bitwise AND with 0b111 is, in this case, a faster equivalent to mod 8
}
int mux_pos(pin_t pin) {
    uint8_t id = (pin >= 38 ? pin-38 : pin+2);
    return (id >> 3);
}

pin_t Muxes::get_pin(pin_t pin) {
    (*muxes[mux_pos(pin)]).get_pin(0);
}

pin_t Muxes::set_pin(pin_t pin) {
    return (*muxes[mux_pos(pin)]).set_pin(mux_id(pin));
}

void Muxes::mode_ADC(pin_t pin) {
    (*muxes[mux_pos(pin)]).mode_ADC(mux_id(pin));
}

void Muxes::mode_VCC(pin_t pin) {
    (*muxes[mux_pos(pin)]).mode_VCC(mux_id(pin));
}

int8_t Muxes::pin_disabled(pin_t pin) {
    return (*muxes[mux_pos(pin)]).pin_disabled(mux_id(pin));
}

void Muxes::setup() {
    pinMode(MUX_INHIBIT_CTR, OUTPUT);
    digitalWrite(MUX_INHIBIT_CTR, HIGH); // digital IO is inhibited on muxed pin
    for (int i=0; i<5; i++) {
        (*muxes[i]).setup();
    }
}

Muxes multiplexer;