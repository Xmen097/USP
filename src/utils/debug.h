#ifndef DEBUG_H
#define DEBUG_H

#include "../globals.h"
#include "../hardware/display.h"
#include "../hardware/input.h"
#include "../app/interface.h"
#include <EEPROM.h>

namespace Debug {
    void voltage_readout();
    void capacity_readout();
    void capacity_raw_readout();
    void calibrate_to_eeprom();
}

#endif