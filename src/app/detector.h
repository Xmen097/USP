#ifndef DETECTOR_H
#define DETECTOR_H

#include "../globals.h"
#include "../chips.h"
#include "../app/interface.h"
#include "../utils/utils.h"
#include "../utils/debug.h"

namespace Detector {
    extern Chip detected_chip;

    void setup();
    bool is_detected();
    void identify_chip();

    void power_up();
    void power_down();
    pin_t get_pin(PinType pin_type);
};

#endif