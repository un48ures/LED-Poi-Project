#include "visual_functions.h"
#include "teensy_4.0_config.h"
// #include "types.h"
#include "FastLED.h"
#include "pic_array.h"

int hue = 0;

// Constants
int default_brightness = 2; // General fix brightness - max brightness 255
unsigned int interval_1 = 1000;      // ms 1st LED Standby blink interval
unsigned int LED_red_on_time = 50;   // ms 1st LED Standby blink on-time
int numberOfSlices = 150;

// LED Variables
bool ledState = LOW;
int fadectr = 0;
int gHue = 128;

// Additional
int looping = 0;
unsigned int previousMillis = 0;
// int ledPin = 13; = SCK PIN bei Teensy 4.0
bool LEDstate = 0;
int blink_once = 0;
unsigned int time_on = 0; // help variable for blinking

// PULSE FADE Variablen
int saturation = 0; // saturation
int value = 0;      // brightness value

extern int fillupDone;
extern int filldownDone;

/// @brief Make decision which array to show in relation to the received message_global
/// @param message_global  contains data which picture/array should be shown
void display(int message_global, int message_brightness, CRGB *leds)
{
  switch (message_global)
  {
  case 0:
    LED_show_color(Black, message_brightness, leds);
    break;
  case 1:
    LED_show_color(White, message_brightness, leds);
    break;
  case 2:
    LED_show_color(Red, message_brightness, leds);
    break;
  case 3:
    LED_show_color(Green, message_brightness, leds);
    break;
  case 4:
    LED_show_color(Blue, message_brightness, leds);
    break;
  case 5:
    LED_show_color(Purple, message_brightness, leds);
    break;
  case 6:
    LED_show_color(Yellow, message_brightness, leds);
    break;
  case 7:
    // FADE PULSE - BLUE
    hue = 150; // BLUE
    LED_Pulsing(hue, message_brightness, leds);
    // delay(50);
    break;
  case 8:
    // FADE PULSE - GREEN
    hue = 100; // GREEN
    LED_Pulsing(hue, message_brightness, leds);
    // delay(50);
    break;
  case 9:
    // FADE PULSE - RED
    hue = 255; // GREEN
    LED_Pulsing(hue, message_brightness, leds);
    // delay(50);
    break;

  //    case 9:
  //      FastLED.setBrightness(5);
  //      if (filldownDone = 0) {
  //        fill_solid(leds, NUM_LEDS, CRGB::Black);
  //      }
  //      LED_filldown();
  //      break;
  case 10:
    FastLED.setBrightness(message_brightness);
    rainbow(message_brightness, leds);
    break;
  case 11:
    LED_strobo(message_brightness, leds);
    break;
  case 12:
    PoiSonic(DEFAULT_TIME, array1, message_brightness, leds);
    break;
  case 13:
    PoiSonic(DEFAULT_TIME, array2, message_brightness, leds);
    break;
  case 14:
    PoiSonic(DEFAULT_TIME, array3, message_brightness, leds);
    break;
  case 15:
    PoiSonic(DEFAULT_TIME, array4, message_brightness, leds);
    break;
  case 16:
    PoiSonic(DEFAULT_TIME, array5, message_brightness, leds);
    break;
  case 17:
    PoiSonic(DEFAULT_TIME, array6, message_brightness, leds);
    break;
  case 18:
    PoiSonic(DEFAULT_TIME, array7, message_brightness, leds);
    break;
  case 19:
    PoiSonic(DEFAULT_TIME, array8, message_brightness, leds);
    break;
  case 20:
    PoiSonic(DEFAULT_TIME, array9, message_brightness, leds);
    break;
  case 21:
    PoiSonic(DEFAULT_TIME, array10, message_brightness, leds);
    break;
  case 22:
    PoiSonic(DEFAULT_TIME, array11, message_brightness, leds);
    break;
  case 23:
    PoiSonic(DEFAULT_TIME, array12, message_brightness, leds);
    break;
  case 24:
    PoiSonic(DEFAULT_TIME, array13, message_brightness, leds);
    break;
  case 25:
    PoiSonic(DEFAULT_TIME, array14, message_brightness, leds);
    break;
  case 26:
    PoiSonic(DEFAULT_TIME, array15, message_brightness, leds);
    break;
  case 27:
    PoiSonic(DEFAULT_TIME, array16, message_brightness, leds);
    break;
  case 28:
    PoiSonic(DEFAULT_TIME, array17, message_brightness, leds);
    break;
  case 29:
    PoiSonic(DEFAULT_TIME, array18, message_brightness, leds);
    break;
  case 30:
    PoiSonic(DEFAULT_TIME, array19, message_brightness, leds);
    break;
  case 31:
    PoiSonic(DEFAULT_TIME, array20, message_brightness, leds);
    break;
  case 32:
    PoiSonic(DEFAULT_TIME, array21, message_brightness, leds);
    break;

  default:
    LED_blink_red(leds);
    break;
  }
}

/// @brief Function to go through the picture arrays and show each slice via FastLED
/// @param time Time a picture is shown (repeated while time is not over)
/// @param array Array (picture) to be shown via FastLED
void PoiSonic_old(unsigned long time, const unsigned int array[], int brightness, CRGB *leds)
{
  unsigned long currentTime = millis();

  while (millis() < currentTime + (time))
  {
    int f = NUM_SLICES;
    int z; // a counter
    int j = NUM_LEDS;
    for (int x = 0; x < f; x++)
    {
      for (z = NUM_LEDS; z > 0; z--)
      {
        leds[z - 1] = array[x + ((j - z) * f)];
      }
      // LED_setBrightness(message_brightness);
      // FastLED.show();
      // Serial.println("FASTLED show...");
      delayMicroseconds(500); // may need to increase / decrease depending on spin rate original value = 40
    }
    delayMicroseconds(1000); // Abstand zwischen Bilder - may need to increase / decrease depending on spin rate
  }
}

void showPicOnce(const unsigned int array[], int brightness, CRGB *leds)
{
  for (int x = 0; x < NUM_SLICES; x++)
  {
    for (int z = NUM_LEDS; z > 0; z--)
    {
      leds[z - 1] = array[x + ((NUM_LEDS - z) * NUM_SLICES)];
    }
    FastLED.setBrightness(brightness);
    FastLED.show();
    // Serial.println("FASTLED show...");
    // if (radio.available()) return;
    delayMicroseconds(500); // Pause between slices - may need to increase / decrease depending on spin rate original value = 40
  }
}

void PoiSonic(unsigned long time, const unsigned int array[], const int brightness, CRGB *leds)
{
  unsigned long currentTime = millis();

  // while ((millis() < currentTime + (time)) && !radio.available())
  while ((millis() < currentTime + (time)))
  {
    showPicOnce(array, brightness, leds);
    delayMicroseconds(1000); // Abstand zwischen Bilder - may need to increase / decrease depending on spin rate
  }
  // if ((millis() < currentTime + (time)) && radio.available())
  if ((millis() < currentTime + (time)))
  {
    Serial.println("Radio available but time not over");
  }
}

// Move a single white dot up and down at startup
void StartDemo(int message_brightness, CRGB *leds)
{
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1)
  {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::White;
    // Show the leds (only one of which is set to white, from above)
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    // Wait a little bit
    delay(7);
    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;
  }

  for (int whiteLed = NUM_LEDS; whiteLed > 0; whiteLed--)
  {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::White;
    // Show the leds (only one of which is set to white, from above)
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);
    FastLED.show();
    // Wait a little bit
    delay(7);
    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;
  }
}

// Standby 1st LED Blink RED
void LED_blink_red(CRGB* leds)
{
  unsigned long currentMillis = millis(); // for LED blink

  if (currentMillis - previousMillis >= interval_1)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    blink_once = 1; // start one blink
  }

  if (blink_once)
  {
    // if the LED is off turn it on and vice-versa:
    if (LEDstate == 0)
    {
      // Let the first LED blink red
      leds[0] = CRGB::Red;
      FastLED.setBrightness(DEFAULT_BRIGHTNESS);
      FastLED.show();
      time_on = millis();
      LEDstate = 1;
    }
    if (LEDstate == 1)
    {
      if ((millis() - time_on) > LED_red_on_time)
      {
        leds[0] = CRGB::Black;
        FastLED.setBrightness(DEFAULT_BRIGHTNESS);
        FastLED.show();
        LEDstate = 0;
        blink_once = 0;
      }
    }
  }
}

// FILL COLORS------------------------------------------------------------------------------------------------
void LED_show_color(HTMLColorCode color, int message_brightness, CRGB *leds)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
  FastLED.setBrightness(message_brightness);
  FastLED.show();
}

//-------------------------------------------------------------------------------------------------------------
// FADE PULSING
//-------------------------------------------------------------------------------------------------------------
void LED_Pulsing(int hue, int message_brightness, CRGB *leds)
{
  // aufsteigend
  if (looping == 0)
  {
    for (int j = 0; j < message_brightness; j++)
    {
      //    if (radio.available()) {last_one = 1;}
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].setHSV(hue, 255, 255);
        FastLED.setBrightness(j);
        FastLED.show();
      }
    }
    looping = 1; // jetzt absteigend
  }

  // absteigend
  if (looping == 1)
  {
    for (int j = message_brightness; j > 0; j--)
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].setHSV(hue, 255, 255);
        FastLED.setBrightness(j);
        FastLED.show();
      }
    }
    looping = 0;
    LED_show_color(Black, message_brightness, leds);
  }
}

//-------------------------------------------------------------------------------------------------------------
// FILL STROBO
//-------------------------------------------------------------------------------------------------------------
void LED_strobo(int message_brightness, CRGB *leds)
{
  LED_show_color(White, message_brightness, leds);
  delay(50);
  LED_show_color(Black, message_brightness, leds);
  delay(50);
}

//-------------------------------------------------------------------------------------------------------------
// FILL FROM BOTTOM
//-------------------------------------------------------------------------------------------------------------
void LED_fillup(int message_brightness, CRGB *leds)
{
  if (fillupDone == 0)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = Red;
      FastLED.setBrightness(message_brightness);
      FastLED.show();
      FastLED.delay(25);
    }
    fillupDone = 1;
  }
}

//-------------------------------------------------------------------------------------------------------------
// FILL FROM TOP
//-------------------------------------------------------------------------------------------------------------
void LED_filldown(int message_brightness, CRGB *leds)
{
  if (filldownDone == 0)
  {
    for (int i = NUM_LEDS - 1; i > 0; i--)
    {
      leds[i] = Red;
      FastLED.setBrightness(message_brightness);
      FastLED.show();
      FastLED.delay(25);
    }
    filldownDone = 1;
  }
}

//-------------------------------------------------------------------------------------------------------------
// RAINBOW
//-------------------------------------------------------------------------------------------------------------
void addGlitter(char chanceOfGlitter, CRGB* leds)
{
  if (random8() < chanceOfGlitter)
  {
    leds[random16(NUM_LEDS)] += White;
  }
}

void rainbow(int message_brightness, CRGB *leds)
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
  addGlitter(80, leds);
  FastLED.setBrightness(message_brightness);
  FastLED.show();
  // FastLED.delay(50); !!!!
  EVERY_N_MILLISECONDS(10)
  {
    gHue++;
  }
}

void fadetoblack(int message_brightness, CRGB *leds)
{
  fadeToBlackBy(leds, NUM_LEDS, 50);
  FastLED.setBrightness(message_brightness);
  FastLED.show();
  FastLED.delay(15);
}

void show_color(message msg, CRGB *leds)
{
  long tmp = msg.picture_hue * 65000;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = tmp;
  }
  FastLED.setBrightness(msg.value_brightness);
  FastLED.show();
}
