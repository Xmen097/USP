#ifndef ERROR_H
#define ERROR_H

#include "../globals.h"

enum class Err {
    IMPLEMENTATION,
    CHIP_DETECT,
    OLED,
};

namespace Error {
    void except(Err error);
    void clear();
};


#endif
