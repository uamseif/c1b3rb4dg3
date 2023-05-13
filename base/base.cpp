/*
    base.h - Base library for C1B3RB4DG3
*/

#include "base.h"

int leds[] = {LED_1_PIN, LED_2_PIN, LED_3_PIN, LED_4_PIN, LED_5_PIN, LED_6_PIN, LED_7_PIN, LED_8_PIN};
bool led_state[] = {false, false, false, false, false, false, false, false};

Arduino_DataBus *bus =  new Arduino_ESP32SPI(2, 5, 18, 23, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_ST7735(bus, 4, 2, false, SCREEN_WIDTH, SCREEN_HEIGHT, 2, 3, 2, 1, false);

ezButton buttonUp(BUTTON_UP_PIN);
ezButton buttonDown(BUTTON_DOWN_PIN);
ezButton buttonLeft(BUTTON_LEFT_PIN);
ezButton buttonRight(BUTTON_RIGHT_PIN);
ezButton buttonCenter(BUTTON_CENTER_PIN);
ezButton buttonA(BUTTON_A_PIN);
ezButton buttonB(BUTTON_B_PIN);

/*
 * Sets up all the pinModes and the screen
 * This function should be called in the setup() function of the main sketch
 */
void setup_badge(){
    pinMode(BUTTON_UP_PIN, INPUT);
    pinMode(BUTTON_DOWN_PIN, INPUT);
    pinMode(BUTTON_LEFT_PIN, INPUT);
    pinMode(BUTTON_RIGHT_PIN, INPUT);
    pinMode(BUTTON_CENTER_PIN, INPUT);
    pinMode(BUTTON_A_PIN, INPUT);
    pinMode(BUTTON_B_PIN, INPUT);

    for (int i = 0; i < ARRAY_SIZE(leds); i++) {
        pinMode(leds[i], OUTPUT);
    }

    gfx->begin();
    gfx->fillScreen(BLACK);
}

/*
 * Lights up the LEDs based on the led_state array
 */
void draw_leds() {
    for (int i = 0; i < ARRAY_SIZE(leds); i++) {
        digitalWrite(leds[i], led_state[i]?HIGH:LOW);
    }
}