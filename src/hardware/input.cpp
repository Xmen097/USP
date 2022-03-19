#include "input.h"

namespace Input {

    const uint32_t input_debounce = 100;

    const pin_t buttons[4] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4};

    enum class change_state {
        BOUNCING,
        STABLE,
        JUST_CHANGED
    };

    bool button_states[4] = {LOW, LOW, LOW, LOW};
    change_state change_states[4] = {change_state::STABLE, change_state::STABLE, change_state::STABLE, change_state::STABLE};
    uint32_t button_times[4] = {0};

    bool debounced(int i) { return (millis() - button_times[i] > input_debounce); }

    void setup() {
        for (int i=0; i<4; i++) {
            pinMode(buttons[i], INPUT_PULLUP);
        }
        loop();
    }

    void loop() {
        bool reading;
        for (int i=0; i<4; i++) {
            reading = !digitalRead(buttons[i]);
            if (reading != button_states[i]) {
                button_times[i] = millis(); 
                button_states[i] = reading;
                change_states[i] = change_state::BOUNCING;
            } else if (change_states[i] == change_state::BOUNCING && debounced(i)) {
                change_states[i] = change_state::JUST_CHANGED;
            } else if (change_states[i] == change_state::JUST_CHANGED) {
                change_states[i] = change_state::STABLE;
            }
        }
    }

    bool just_pressed(Button btn) {
        if (btn == Button::ANY) {
            for (uint8_t i=0; i<4; i++)
                if (just_pressed((Button)i)) return true;
            return false;
        } 
        return (change_states[(uint8_t)btn] == change_state::JUST_CHANGED) && button_states[(uint8_t)btn];
    }

    bool just_released(Button btn) {
        if (btn == Button::ANY) {
            for (uint8_t i=0; i<4; i++)
                if (just_released((Button)i)) return true;
            return false;
        } 
        return (change_states[(uint8_t)btn] == change_state::JUST_CHANGED) && !button_states[(uint8_t)btn];
    }

    bool is_down(Button btn) {
        if (btn == Button::ANY) {
            for (uint8_t i=0; i<4; i++)
                if (is_down((Button)i)) return true;
            return false;
        } 
        return (change_states[(uint8_t)btn] != change_state::BOUNCING) && button_states[(uint8_t)btn];
    }
    
    void wait_until(Button btn) {
        loop();
        while(!just_released(btn)) {loop();}
    }

}