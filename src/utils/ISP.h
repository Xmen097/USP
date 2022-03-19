#ifndef ISP_H
#define ISP_H

#include "../globals.h"
#include "../app/detector.h"

#define LED_ERR   LEDR
#define LED_PMODE LEDB

namespace ISP {
    void setup();
    void loop();
};

#endif