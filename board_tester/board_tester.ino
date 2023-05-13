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

#define LED_1_PIN                           22
#define LED_2_PIN                           21
#define LED_3_PIN                           17
#define LED_4_PIN                           16
#define LED_5_PIN                           14
#define LED_6_PIN                           12
#define LED_7_PIN                           13
#define LED_8_PIN                           15

#define SCREEN_WIDTH                        128
#define SCREEN_HEIGHT                       160

int led_list[] = {LED_1_PIN, LED_2_PIN, LED_3_PIN, LED_4_PIN, LED_5_PIN, LED_6_PIN, LED_7_PIN, LED_8_PIN};
int current_led = 0;
unsigned long last_millis = 0;
int led_period = 500;

Arduino_DataBus *bus = new Arduino_ESP32SPI(2, 5, 18, 23, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_ST7735(bus, 4, 2, false, SCREEN_WIDTH, SCREEN_HEIGHT, 2, 3, 2, 1, false);

ezButton buttonUp(BUTTON_UP_PIN);
ezButton buttonDown(BUTTON_DOWN_PIN);
ezButton buttonLeft(BUTTON_LEFT_PIN);
ezButton buttonRight(BUTTON_RIGHT_PIN);
ezButton buttonCenter(BUTTON_CENTER_PIN);
ezButton buttonA(BUTTON_A_PIN);
ezButton buttonB(BUTTON_B_PIN);

bool up_pressed = false;
bool down_pressed = false;
bool left_pressed = false;
bool right_pressed = false;
bool center_pressed = false;
bool a_pressed = false;
bool b_pressed = false;
bool text_printed = false;

int screen_offset = 20;

int32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
uint8_t tsa, tsb, tsc, ds;

void setup() {
  pinMode(BUTTON_UP_PIN, INPUT);
  pinMode(BUTTON_DOWN_PIN, INPUT);
  pinMode(BUTTON_LEFT_PIN, INPUT);
  pinMode(BUTTON_RIGHT_PIN, INPUT);
  pinMode(BUTTON_CENTER_PIN, INPUT);
  pinMode(BUTTON_A_PIN, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);

  for (int i = 0; i < ARRAY_SIZE(led_list); i++) {
    pinMode(led_list[i], OUTPUT);
  }

  gfx->begin();
  gfx->fillScreen(BLACK);
  last_millis = millis();
}

void draw_leds() {
  if (millis() - last_millis >= led_period) {
    last_millis = millis();
    current_led ++;
    if (current_led >= ARRAY_SIZE(led_list)) {
      current_led = 0;
    }

    for (int i = 0; i < ARRAY_SIZE(led_list); i++) {
      if (i == current_led) {
        digitalWrite(led_list[i], HIGH);
      } else {
        digitalWrite(led_list[i], LOW);
      }
    }
  }
}

void buttonsLoop() {
  buttonUp.loop();
  buttonDown.loop();
  buttonLeft.loop();
  buttonRight.loop();
  buttonCenter.loop();
  buttonA.loop();
  buttonB.loop();
}

void draw_buttons() {
  buttonsLoop();

  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
  bool center = false;
  bool a = false;
  bool b = false;


  up = buttonUp.isPressed();
  down = buttonDown.isPressed();
  left = buttonLeft.isPressed();
  right = buttonRight.isPressed();
  center = buttonCenter.isPressed();
  a = buttonA.isPressed();
  b = buttonB.isPressed();

  if (up) {
    up_pressed = true;
  }

  if (down) {
    down_pressed = true;
  }

  if (left) {
    left_pressed = true;
  }

  if (right) {
    right_pressed = true;
  }

  if (center) {
    center_pressed = true;
  }

  if (a) {
    a_pressed = true;
  }

  if (b) {
    b_pressed = true;
  }

  if (up_pressed && down_pressed && left_pressed && right_pressed && center_pressed && a_pressed && b_pressed) {
    text_printed = true;
  }

  if (!up_pressed) {
    gfx->drawRect(screen_offset + 10, SCREEN_HEIGHT - screen_offset - 30, 10, 10, WHITE);
  } else {
    gfx->fillRect(screen_offset + 10, SCREEN_HEIGHT - screen_offset - 30, 10, 10, WHITE);
  }

  if (!down_pressed) {
    gfx->drawRect(screen_offset + 10, SCREEN_HEIGHT - screen_offset - 10, 10, 10, WHITE);
  } else {
    gfx->fillRect(screen_offset + 10, SCREEN_HEIGHT - screen_offset - 10, 10, 10, WHITE);
  }

  if (!left_pressed) {
    gfx->drawRect(screen_offset, SCREEN_HEIGHT - screen_offset - 20, 10, 10, WHITE);
  } else {
    gfx->fillRect(screen_offset, SCREEN_HEIGHT - screen_offset - 20, 10, 10, WHITE);
  }

  if (!right_pressed) {
    gfx->drawRect(screen_offset + 20, SCREEN_HEIGHT - screen_offset - 20, 10, 10, WHITE);
  } else {
    gfx->fillRect(screen_offset + 20, SCREEN_HEIGHT - screen_offset - 20, 10, 10, WHITE);
  }

  if (!center_pressed) {
    gfx->drawRect(screen_offset + 10, SCREEN_HEIGHT - screen_offset - 20, 10, 10, WHITE);
  } else {
    gfx->fillRect(screen_offset + 10, SCREEN_HEIGHT - screen_offset - 20, 10, 10, WHITE);
  }

  if (!b_pressed) {
    gfx->drawRect(SCREEN_WIDTH - screen_offset - 25, SCREEN_HEIGHT - screen_offset - 15, 10, 10, WHITE);
  } else {
    gfx->fillRect(SCREEN_WIDTH - screen_offset - 25, SCREEN_HEIGHT - screen_offset - 15, 10, 10, WHITE);
  }

  if (!a_pressed) {
    gfx->drawRect(SCREEN_WIDTH - screen_offset - 10, SCREEN_HEIGHT - screen_offset - 20, 10, 10, WHITE);
  } else {
    gfx->fillRect(SCREEN_WIDTH - screen_offset - 10, SCREEN_HEIGHT - screen_offset - 20, 10, 10, WHITE);
  }

  if (!text_printed) {
    gfx->setCursor(10, 20);
    gfx->setTextColor(WHITE);
    gfx->print("Buttons test: ");
    gfx->setTextColor(RED);
    gfx->print("NOK");
  } else {
    gfx->fillRect(0, 0, SCREEN_WIDTH, 30, BLACK);
    gfx->setCursor(10, 20);
    gfx->setTextColor(WHITE);
    gfx->print("Buttons test: ");
    gfx->setTextColor(GREEN);
    gfx->print("OK");
    delay(3000);
  }
}

void draw_screen_test() {
  w = gfx->width();
  h = gfx->height();
  n = min(w, h);
  n1 = n - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = cn - 1;
  tsa = ((w <= 176) || (h <= 160)) ? 1 : (((w <= 240) || (h <= 240)) ? 2 : 3); // text size A
  tsb = ((w <= 272) || (h <= 220)) ? 1 : 2;                                    // text size B
  tsc = ((w <= 220) || (h <= 220)) ? 1 : 2;                                    // text size C
  ds = (w <= 160) ? 9 : 12;                                                    // digit size


  testFillScreen();
  testText();
  testPixels();
  testLines();
  testFastLines();
  testFilledRects();
  testRects();
  testFilledTriangles();
  testTriangles();
  testFilledCircles(10);
  testCircles(10);
  testFillArcs();
  testArcs();
  testFilledRoundRects();
  testRoundRects();

}

void loop() {
  draw_leds();
  if (!text_printed) {
    draw_buttons();
  } else {
    draw_screen_test();
  }
}

static inline uint32_t micros_start() __attribute__((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms)
    ;
  return micros();
}


int32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  gfx->fillScreen(WHITE);
  gfx->fillScreen(RED);
  gfx->fillScreen(GREEN);
  gfx->fillScreen(BLUE);
  gfx->fillScreen(BLACK);

  return micros() - start;
}

int32_t testText()
{
  uint32_t start = micros_start();
  gfx->setCursor(0, 0);

  gfx->setTextSize(1);
  gfx->setTextColor(WHITE, BLACK);
  gfx->println(F("Hello World!"));

  gfx->setTextSize(2);
  gfx->setTextColor(gfx->color565(0xff, 0x00, 0x00));
  gfx->print(F("RED "));
  gfx->setTextColor(gfx->color565(0x00, 0xff, 0x00));
  gfx->print(F("GREEN "));
  gfx->setTextColor(gfx->color565(0x00, 0x00, 0xff));
  gfx->println(F("BLUE"));

  gfx->setTextSize(tsa);
  gfx->setTextColor(YELLOW);
  gfx->println(1234.56);

  gfx->setTextColor(WHITE);
  gfx->println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);

  gfx->setTextColor(CYAN, WHITE);
  gfx->println(F("Groop,"));

  gfx->setTextSize(tsc);
  gfx->setTextColor(MAGENTA, WHITE);
  gfx->println(F("I implore thee,"));

  gfx->setTextSize(1);
  gfx->setTextColor(NAVY, WHITE);
  gfx->println(F("my foonting turlingdromes."));

  gfx->setTextColor(DARKGREEN, WHITE);
  gfx->println(F("And hooptiously drangle me"));

  gfx->setTextColor(DARKCYAN, WHITE);
  gfx->println(F("with crinkly bindlewurdles,"));

  gfx->setTextColor(MAROON, WHITE);
  gfx->println(F("Or I will rend thee"));

  gfx->setTextColor(PURPLE, WHITE);
  gfx->println(F("in the gobberwartsb"));

  gfx->setTextColor(OLIVE, WHITE);
  gfx->println(F("with my blurglecruncheon,"));

  gfx->setTextColor(DARKGREY, WHITE);
  gfx->println(F("see if I don't!"));

  gfx->setTextSize(2);
  gfx->setTextColor(RED);
  gfx->println(F("Size 2"));

  gfx->setTextSize(3);
  gfx->setTextColor(ORANGE);
  gfx->println(F("Size 3"));

  gfx->setTextSize(4);
  gfx->setTextColor(YELLOW);
  gfx->println(F("Size 4"));

  gfx->setTextSize(5);
  gfx->setTextColor(GREENYELLOW);
  gfx->println(F("Size 5"));

  gfx->setTextSize(6);
  gfx->setTextColor(GREEN);
  gfx->println(F("Size 6"));

  gfx->setTextSize(7);
  gfx->setTextColor(BLUE);
  gfx->println(F("Size 7"));

  gfx->setTextSize(8);
  gfx->setTextColor(PURPLE);
  gfx->println(F("Size 8"));

  gfx->setTextSize(9);
  gfx->setTextColor(PINK);
  gfx->println(F("Size 9"));

  return micros() - start;
}

int32_t testPixels()
{
  uint32_t start = micros_start();

  for (int16_t y = 0; y < h; y++)
  {
    for (int16_t x = 0; x < w; x++)
    {
      gfx->drawPixel(x, y, gfx->color565(x << 3, y << 3, x * y));
    }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif
  }

  return micros() - start;
}

int32_t testLines()
{
  uint32_t start;
  int32_t x1, y1, x2, y2;

  start = micros_start();

  x1 = y1 = 0;
  y2 = h - 1;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = 0;
  y1 = h - 1;
  y2 = 0;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  return micros() - start;
}

int32_t testFastLines()
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  for (y = 0; y < h; y += 5)
  {
    gfx->drawFastHLine(0, y, w, RED);
  }
  for (x = 0; x < w; x += 5)
  {
    gfx->drawFastVLine(x, 0, h, BLUE);
  }

  return micros() - start;
}

int32_t testFilledRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    gfx->fillRect(cx - i2, cy - i2, i, i, gfx->color565(i, i, 0));
  }

  return micros() - start;
}

int32_t testRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    gfx->drawRect(cx - i2, cy - i2, i, i, GREEN);
  }

  return micros() - start;
}

int32_t testFilledCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      gfx->fillCircle(x, y, radius, MAGENTA);
    }
  }

  return micros() - start;
}

int32_t testCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;
  int32_t w1 = w + radius;
  int32_t h1 = h + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros_start();

  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      gfx->drawCircle(x, y, radius, WHITE);
    }
  }

  return micros() - start;
}

int32_t testFillArcs()
{
  int16_t i, r = 360 / cn;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    gfx->fillArc(cx1, cy1, i, i - 3, 0, i * r, RED);
  }

  return micros() - start;
}

int32_t testArcs()
{
  int16_t i, r = 360 / cn;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    gfx->drawArc(cx1, cy1, i, i - 3, 0, i * r, WHITE);
  }

  return micros() - start;
}

int32_t testFilledTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = cn1; i > 10; i -= 5)
  {
    gfx->fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      gfx->color565(0, i, i));
  }

  return micros() - start;
}

int32_t testTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = 0; i < cn; i += 5)
  {
    gfx->drawTriangle(
            cx1, cy1 - i,     // peak
            cx1 - i, cy1 + i, // bottom left
            cx1 + i, cy1 + i, // bottom right
            gfx->color565(0, 0, i));
  }

  return micros() - start;
}

int32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    gfx->fillRoundRect(cx - i2, cy - i2, i, i, i / 8, gfx->color565(0, i, 0));
  }

  return micros() - start;
}

int32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = 20; i < n1; i += 6)
  {
    i2 = i / 2;
    gfx->drawRoundRect(cx - i2, cy - i2, i, i, i / 8, gfx->color565(i, 0, 0));
  }

  return micros() - start;
}
