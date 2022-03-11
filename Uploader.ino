#include "src/hardware/input.h"
#include "src/hardware/display.h"
#include "src/app/detector.h"
#include "src/utils/debug.h"
#include "src/utils/ISP.h"

void setup() {
    Display::setup();
    Detector::setup();
    Input::setup();
    Serial.begin(19200);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    pinMode(LEDR, OUTPUT);
    digitalWrite(LEDG, HIGH);
    //Interface::apply_diode_voltage(5);
    //ISP::setup();
    delay(500);
}

void loop() {
    Display::write(F("Ready"));
    while(!Detector::is_detected());
    Detector::identify_chip();
    Detector::power_up();
    ISP::setup();
    Display::write(Detector::detected_chip.name);
    Input::loop();
    while(!Input::is_down(Button::OK)) {
        Input::loop();
        ISP::loop();
    }
    Detector::power_down();
}
