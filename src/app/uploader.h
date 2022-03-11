#ifndef UPLOADER_H
#define UPLOADER_H

#include "../globals.h"
#include "../chips.h"

class Uploader {
    public:
        Uploader() {};
        pin_t resolve_pin_type(PinType pin);
        void write(PinType pin, bool state);
        void mode(PinType pin, bool state);
        bool read(PinType pin);
    private:
};

#endif