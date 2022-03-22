#include "src/hardware/input.h"
#include "src/hardware/display.h"
#include "src/app/detector.h"
#include "src/utils/debug.h"
#include "src/utils/ISP.h"

#define BAUDRATE  19200
// #define BAUDRATE 115200
// #define BAUDRATE 1000000

void setup() {
    Display::setup();
    Detector::setup();
    Input::setup();
    Serial.begin(BAUDRATE);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    pinMode(LEDR, OUTPUT);
    digitalWrite(LEDG, HIGH);
    delay(500);
}

void loop() {
    Display::write(F("Ready"));
    while(!Detector::is_detected());
    Display::write(F("Detecting"));
    Detector::identify_chip();
    Detector::power_up();
    ISP::setup();
    Display::write(Detector::detected_chip.name);
    Input::loop();
    while(!Input::is_down(Button::ANY)) {
        Input::loop();
        ISP::loop();
    }
    Detector::power_down();
}
