#include "types.h"
#include <FastLED.h>

#define DEFAULT_TIME 25
#define NUM_SLICES 150

void display(int message_global, int message_brightness, CRGB* leds);
void PoiSonic_old(unsigned long time, const unsigned int array[], int brightness, CRGB* leds);
void showPicOnce(const unsigned int array[], int brightness, CRGB* leds);
void PoiSonic(unsigned long time, const unsigned int array[], int brightness, CRGB* leds);
void StartDemo(int message_brightness, CRGB* leds);
void LED_blink_red(CRGB* leds);
void LED_show_color(HTMLColorCode color, int brightness, CRGB* leds);
void LED_Pulsing(int hue, int message_brightness);
void LED_strobo(int message_brightness, CRGB* leds);
void LED_fillup(int message_brightness, CRGB* leds);
void LED_filldown(int message_brightness, CRGB* leds);
void addGlitter(char chanceOfGlitter, CRGB* leds);
void rainbow(int message_brightness, CRGB* leds);
void fadetoblack(int message_brightness, CRGB* leds);
void LED_Pulsing(int hue, int message_brightness, CRGB* leds);
void show_color(message msg, CRGB* leds);