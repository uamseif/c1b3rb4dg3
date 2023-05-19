/*
    base.h - Base library for C1B3RB4DG3
*/

#include "base.h"

int leds[] = {LED_1_PIN, LED_2_PIN, LED_3_PIN, LED_4_PIN, LED_5_PIN, LED_6_PIN, LED_7_PIN, LED_8_PIN};
bool ledState[] = {false, false, false, false, false, false, false, false};

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
void setupBadge(){
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
 * Lights up the LEDs based on the ledState array
 */
void drawLeds() {
    for (int i = 0; i < ARRAY_SIZE(leds); i++) {
        digitalWrite(leds[i], ledState[i]?HIGH:LOW);
    }
}

/*
 * Turns off all the LEDs
 */
void clearLeds() {
    for (int i = 0; i < ARRAY_SIZE(leds); i++) {
        digitalWrite(leds[i], LOW);
    }
}

/*
 * Turns off all the LEDs
 */
void clear_leds() {
    for (int i = 0; i < ARRAY_SIZE(leds); i++) {
        digitalWrite(leds[i], LOW);
    }
}

/*
 * Draws the c1b3rtr4cks logo, then the SEIF logo with a glitch effect
 */
void splashScreen() {
	gfx->fillScreen(BLACK);
	gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)logoCibertracks, 128, 160);
	delay(1500);

	gfx->fillScreen(BLACK);
	gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)logoSEIF, 128, 160);
	delay(500);
	gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)logoSEIFGlitch, 128, 160);
	delay(150);
	gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)logoSEIF, 128, 160);
	delay(800);
	gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)logoSEIFGlitch, 128, 160);
	delay(100);
	gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)logoSEIF, 128, 160);
	delay(800);
	
    gfx->fillScreen(BLACK);
}

/*
 * Calls loop() method on all the buttons
 */
void buttonsLoop() {
  buttonUp.loop();
  buttonDown.loop();
  buttonLeft.loop();
  buttonRight.loop();
  buttonCenter.loop();
  buttonA.loop();
  buttonB.loop();
}