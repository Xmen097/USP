#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "../globals.h"

// Multiplexer Interface
class Multiplexer {
    public:
        virtual ~Multiplexer() {}
        virtual pin_t set_pin(uint8_t id) = 0;	// This function has hardware-related side effects on other muxes in the same line.
        virtual pin_t get_pin(uint8_t id) = 0;
        virtual void mode_ADC(uint8_t id) = 0;
        virtual void mode_VCC(uint8_t id) = 0;
        virtual int8_t pin_disabled(uint8_t id) = 0; // returns -1 if pin is enabled, otherwise returns id of alternative pin 
        virtual void setup() = 0;
};

class Muxes : public Multiplexer { // Composite class
    private:
        Multiplexer *muxes[5];		

    public:
        pin_t get_pin(uint8_t id) override;         // Returns multiplexer ADC pin
        pin_t set_pin(uint8_t id) override;         // Connects multiplexer to pin, returns multiplexer ADC pin
        void mode_ADC(uint8_t id) override;         // Connects ADC to multiplexer
        void mode_VCC(uint8_t id) override;         // Connects VCC (12V) to multiplexer
        int8_t pin_disabled(uint8_t id) override;   // Is digital IO overriden by multiplexer
        void setup() override;
        Muxes();
};

extern Muxes multiplexer;

#endif