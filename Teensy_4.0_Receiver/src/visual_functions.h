#ifndef VISUAL_FUNCTIONS
#define VISUAL_FUNCTIONS

#include "types.h"
#include <FastLED.h>

typedef struct RGBColour {
	int r;
	int g;
	int b;
} RGBColour;


void display(message msg, CRGB* leds);
void PoiSonic_old(unsigned long time, const unsigned int array[], int brightness, CRGB* leds);
void showPicOnce(const unsigned int array[], int array_length, message msg, CRGB* leds);
void StartDemo(int message_brightness, CRGB* leds);
void LED_blink_red(CRGB* leds);
void LED_show_color(HTMLColorCode color, int brightness, CRGB* leds);
void LED_strobo(int message_brightness, int velocity, CRGB* leds);
void LED_fillup(message msg, bool reverse, CRGB* leds);
void LED_runner(message msg, bool reverse, CRGB* leds);
void addGlitter(char chanceOfGlitter, CRGB* leds);
void rainbow(message msg, CRGB* leds);
void juggle(message msg, CRGB *leds);
void bpm(message msg, CRGB *leds);
void sinelon(message msg, CRGB *leds);
void confetti(message msg, CRGB *leds);
void pride(CRGB *leds);
void Fire2012(CRGB* leds);
void dim_up(message msg, CRGB* leds);
void dim_down(message msg, CRGB* leds);
void LED_Pulsing(int hue, int message_brightness, int velocity, CRGB* leds);
void show_color(message msg, CRGB* leds);
RGBColour hsv2rgb(float H, float S, float V);
#endif