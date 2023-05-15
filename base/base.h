/*
    base.h - Base library for C1B3RB4DG3
*/
#ifndef base_c1b3rb4dg3_h
#define base_c1b3rb4dg3_h

#include <Arduino_GFX_Library.h>
#include <ezButton.h>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

#define BUTTON_UP_PIN                       35
#define BUTTON_DOWN_PIN                     32
#define BUTTON_LEFT_PIN                     19
#define BUTTON_RIGHT_PIN                    27
#define BUTTON_CENTER_PIN                   33
#define BUTTON_A_PIN                        25
#define BUTTON_B_PIN                        26

#define SCREEN_WIDTH                        128
#define SCREEN_HEIGHT                       160

#define LED_1_PIN                           22
#define LED_2_PIN                           21
#define LED_3_PIN                           17
#define LED_4_PIN                           16
#define LED_5_PIN                           14
#define LED_6_PIN                           12
#define LED_7_PIN                           13
#define LED_8_PIN                           15

extern int leds[];
extern bool led_state[];

extern Arduino_DataBus *bus; // Arduino_ESP32SPI
extern Arduino_GFX *gfx; // Arduino_ST7735

/* Declare all the buttons in the badge */
extern ezButton buttonUp;
extern ezButton buttonDown;
extern ezButton buttonLeft;
extern ezButton buttonRight;
extern ezButton buttonCenter;
extern ezButton buttonA;
extern ezButton buttonB;

/*
 * Sets up all the pinModes and the screen
 * This function should be called in the setup() function of the main sketch
 */
void setup_badge();

/*
 * Lights up the LEDs based on the led_state array
 */
void draw_leds();

/*
 * Draws the c1b3rtr4cks logo, then the SEIF logo with a glitch effect
 */
void splash_screen();

#endif