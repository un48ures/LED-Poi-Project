#ifndef VISUAL_FUNCTIONS
#define VISUAL_FUNCTIONS

#include "types.h"
#include <FastLED.h>

#define DEFAULT_TIME 25
#define NUM_SLICES 150

typedef struct RGBColour {
	int r;
	int g;
	int b;
} RGBColour;


void display(message msg, CRGB* leds);
void PoiSonic_old(unsigned long time, const unsigned int array[], int brightness, CRGB* leds);
void showPicOnce(const unsigned int array[], int brightness, CRGB* leds);
void PoiSonic(unsigned long time, const unsigned int array[], int brightness, CRGB* leds);
void StartDemo(int message_brightness, CRGB* leds);
void LED_blink_red(CRGB* leds);
void LED_show_color(HTMLColorCode color, int brightness, CRGB* leds);
void LED_strobo(int message_brightness, int velocity, CRGB* leds);
void LED_fillup(int message_brightness, CRGB* leds);
void LED_filldown(int message_brightness, CRGB* leds);
void addGlitter(char chanceOfGlitter, CRGB* leds);
void rainbow(int message_brightness, CRGB* leds);
void fadetoblack(int message_brightness, CRGB* leds);
void LED_Pulsing(int hue, int message_brightness, int velocity, CRGB* leds);
void show_color(message msg, CRGB* leds);
RGBColour hsv2rgb(float H, float S, float V);
#endif