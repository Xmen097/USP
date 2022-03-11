#ifndef INPUT_H
#define INPUT_H

#include "../globals.h"

enum class Button {
    LEFT,
    RIGHT,
    OK,
    BACK
};

namespace Input {
    bool just_pressed(Button btn);
    bool just_released(Button btn);
    bool is_down(Button btn);
    void wait_until(Button btn);
    void setup();
    void loop();
};

#endif