#include "display.h"
#include "../utils/error.h"

namespace Display {

    const uint8_t screen_width = 128;
    const uint8_t screen_height = 32;

    Adafruit_SSD1306 display(screen_width, screen_height, &Wire);

    GFXcanvas1 canvas(128, 32);

    void setup() {
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Display address is 0x3C 
            Error::except(Err::OLED);
        }
        display.display();
        delay(500);
        display.clearDisplay();
        display.drawBitmap(0, 0, uploader_logo, 128, 32, SSD1306_WHITE);
        display.display();
        display.setTextColor(SSD1306_WHITE);
    }

    void debug_info(uint16_t *data) {
        canvas.fillScreen(0);
        canvas.setFont(&Font5x5Fixed);
        canvas.setRotation(0);
        canvas.drawBitmap(0, 15, debug_spliter, 128, 2, SSD1306_WHITE);
        canvas.setRotation(1);
        
        for (int i=0; i<40; i++) {
            uint8_t x = i < 20 ? 1 : 0;
            uint8_t y = i < 20 ? 19-i : i-20;
            canvas.setCursor(x*18, 7+6*y);
            canvas.print(data[i]);
        }
        display.clearDisplay();
        display.drawBitmap(0, 0, canvas.getBuffer(), 128, 32, SSD1306_WHITE);
        display.display();
    }

    void write_setup() {
        display.clearDisplay();
        display.setRotation(0);
        display.setFont();
        display.setTextSize(1);
    }

    void write(const __FlashStringHelper * text) { // prints text saved in EEPROM
        write_setup();
        display.setCursor(0, 8);
        display.println(text);
        display.display();
    }    
    void write(const char* text) { // prints text saved in EEPROM
        write_setup();
        display.setCursor(0, 8);
        display.println(text);
        display.display();
    }
    void write(const uint32_t number) { // prints text saved in EEPROM
        char buffer [17];
        itoa (number,buffer,10);
        write_setup();
        display.setCursor(0, 8);
        display.println(buffer);
        display.display();
    }

    void clear() {
        display.clearDisplay();
        display.display();
    }

}