#include "visual_functions.h"
#include "teensy_4.0_config.h"
// #include "types.h"
#include "FastLED.h"
#include "pic_array.h"
#include <RF24.h>

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

bool fillupDone = false;
bool filldownDone = false;
bool dim_up_done = false;
bool picture_changed = false;
unsigned long starttime = 0;

/// @brief Make decision which array to show in relation to the received message_global
/// @param message_global  contains data which picture/array should be shown
void display(message msg, CRGB *leds)
{
  static int old_picture = 0;
  picture_changed = false;

  if (msg.picture != old_picture)
  {
    picture_changed = true;
    filldownDone = false;
    fillupDone = false;
    dim_up_done = false;
  }

  switch (msg.picture)
  {
  case 0:
    LED_show_color(Black, msg.value_brightness, leds);
    break;
  case 1:
    LED_show_color(White, msg.value_brightness, leds);
    break;
  case 2:
    LED_Pulsing(msg.hue, msg.value_brightness, msg.velocity, leds);
    break;
  case 3:
    LED_strobo(msg.value_brightness, msg.velocity, leds);
    break;
  case 4:
    FastLED.setBrightness(msg.value_brightness);
    rainbow(msg.value_brightness, leds);
    break;
  case 5: // FILL DOWN
    FastLED.setBrightness(5);
    if (filldownDone == 0) {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
    }
    LED_filldown(msg, leds);
    break;
  case 6: // FILL UP
    FastLED.setBrightness(5);
    if (fillupDone == 0) {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
    }
    LED_fillup(msg, leds);
    break;
  case 7: // DIM UP
    dim_up(msg, leds);
    break;
  case 8: // DIM DOWN
    dim_down(msg, leds);
    break;
  case 12:
    PoiSonic(DEFAULT_TIME, array1, msg.value_brightness, leds);
    break;
  case 13:
    PoiSonic(DEFAULT_TIME, array2, msg.value_brightness, leds);
    break;
  case 14:
    PoiSonic(DEFAULT_TIME, array3, msg.value_brightness, leds);
    break;
  case 15:
    PoiSonic(DEFAULT_TIME, array4, msg.value_brightness, leds);
    break;
  case 16:
    PoiSonic(DEFAULT_TIME, array5, msg.value_brightness, leds);
    break;
  case 17:
    PoiSonic(DEFAULT_TIME, array6, msg.value_brightness, leds);
    break;
  case 18:
    PoiSonic(DEFAULT_TIME, array7, msg.value_brightness, leds);
    break;
  case 19:
    PoiSonic(DEFAULT_TIME, array8, msg.value_brightness, leds);
    break;
  case 20:
    PoiSonic(DEFAULT_TIME, array9, msg.value_brightness, leds);
    break;
  case 21:
    PoiSonic(DEFAULT_TIME, array10, msg.value_brightness, leds);
    break;
  case 22:
    PoiSonic(DEFAULT_TIME, array11, msg.value_brightness, leds);
    break;
  case 23:
    PoiSonic(DEFAULT_TIME, array12, msg.value_brightness, leds);
    break;
  case 24:
    PoiSonic(DEFAULT_TIME, array13, msg.value_brightness, leds);
    break;
  case 25:
    PoiSonic(DEFAULT_TIME, array14, msg.value_brightness, leds);
    break;
  case 26:
    PoiSonic(DEFAULT_TIME, array15, msg.value_brightness, leds);
    break;
  case 27:
    PoiSonic(DEFAULT_TIME, array16, msg.value_brightness, leds);
    break;
  case 28:
    PoiSonic(DEFAULT_TIME, array17, msg.value_brightness, leds);
    break;
  case 29:
    PoiSonic(DEFAULT_TIME, array18, msg.value_brightness, leds);
    break;
  case 30:
    PoiSonic(DEFAULT_TIME, array19, msg.value_brightness, leds);
    break;
  case 31:
    PoiSonic(DEFAULT_TIME, array20, msg.value_brightness, leds);
    break;
  case 32:
    PoiSonic(DEFAULT_TIME, array21, msg.value_brightness, leds);
    break;

  default:
    LED_blink_red(leds);
    break;
  }
  old_picture = msg.picture;
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
  unsigned long startTime = millis();

  // while ((millis() < startTime + (time)) && !radio.available())
  while ((millis() < startTime + (time)))
  {
    showPicOnce(array, brightness, leds);
    delayMicroseconds(1000); // Abstand zwischen Bilder - may need to increase / decrease depending on spin rate
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
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Black;
      }
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
        for (int i = 0; i < NUM_LEDS; i++)
        {
          leds[i] = CRGB::Black;
        }
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
void LED_Pulsing(int hue, int message_brightness, int velocity, CRGB *leds)
{
  if (picture_changed)
  {
    starttime = millis();
    looping = 0;
  }

  int b_target = message_brightness;
  if (message_brightness < 10)
  {
    b_target = 10;
  }
  // Aufsteigend
  if (looping == 0)
  {
    if(millis() < (starttime + velocity * 40))
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].setHSV(hue, 255, 255);
        FastLED.setBrightness((uint8_t) ((millis() - starttime) / (velocity * 40.0) * (float) b_target));
        FastLED.show();
      }
    }
    else
    {
      looping = 1; // jetzt absteigend
      starttime = millis();
    }
  }

  // Absteigend
  if (looping == 1)
  {
    if(millis() < (starttime + velocity * 40))
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].setHSV(hue, 255, 255);
        FastLED.setBrightness((uint8_t) ((starttime + velocity * 40.0 - millis()) / (velocity * 40.0) * (float) b_target));
        FastLED.show();
      }
    }
    else
    {
      looping = 0;
      starttime = millis();
    }
  }
}

//-------------------------------------------------------------------------------------------------------------
// FILL STROBO
//-------------------------------------------------------------------------------------------------------------
void LED_strobo(int message_brightness, int velocity, CRGB *leds)
{
  if(picture_changed)
  {
    starttime = millis();
  }
  
  if(millis() < (starttime + velocity))
  {
    LED_show_color(White, message_brightness, leds);
  }
  else
  {
    if(millis() < (starttime + 2 * velocity))
    {
      LED_show_color(Black, message_brightness, leds);
    }
    else
    {
      starttime = millis();
    }
  }
  
  //delay(velocity);
  
  // delay(velocity);
}

//-------------------------------------------------------------------------------------------------------------
// FILL FROM BOTTOM
//-------------------------------------------------------------------------------------------------------------
void LED_fillup(message msg, CRGB *leds)
{
  if (fillupDone == 0)
  {
    RGBColour c = hsv2rgb(msg.hue / 255.0 * 360.0, 100, 100); // transform 255 to 360° hue, ignore saturation and brightness

    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i].r = c.r;
      leds[i].g = c.g;
      leds[i].b = c.b;
      FastLED.setBrightness(msg.value_brightness);
      FastLED.show();
      FastLED.delay(msg.velocity);
    }
    fillupDone = 1;
  }
}

//-------------------------------------------------------------------------------------------------------------
// FILL FROM TOP
//-------------------------------------------------------------------------------------------------------------
void LED_filldown(message msg, CRGB *leds)
{
  RGBColour c = hsv2rgb(msg.hue / 255.0 * 360.0, 100, 100); // transform 255 to 360° hue, ignore saturation and brightness

  if (filldownDone == 0)
  {
    for (int i = NUM_LEDS - 1; i > 0; i--)
    {
      leds[i].r = c.r;
      leds[i].g = c.g;
      leds[i].b = c.b;
      FastLED.setBrightness(msg.value_brightness);
      FastLED.show();
      FastLED.delay(msg.velocity);
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

void dim_down(message msg, CRGB *leds)
{
  fadeToBlackBy(leds, NUM_LEDS, 50);
  FastLED.setBrightness(msg.value_brightness);
  FastLED.show();
  FastLED.delay(msg.velocity);
}

void dim_up(message msg, CRGB *leds) // need implementation
{
  if (dim_up_done == false)
  {
    show_color(msg, leds);
    for (int i = 0; i < msg.value_brightness; i++)
    {
      FastLED.setBrightness(i);
      FastLED.show();
      FastLED.delay(msg.velocity);
    }
    dim_up_done = true;
  }
}

void show_color(message msg, CRGB *leds)
{
  // long tmp = msg.picture_hue * 65000;
  // printf("msg.picture_hue = %d\n", msg.picture_hue);
  RGBColour c = hsv2rgb(msg.hue / 255.0 * 360.0, 100, 100); // transform 255 to 360° hue, ignore saturation and brightness

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].r = c.r;
    leds[i].g = c.g;
    leds[i].b = c.b;
  }
  FastLED.setBrightness(msg.value_brightness);
  FastLED.show();
}

RGBColour hsv2rgb(float H, float S, float V) {
	float r = 0.0, g = 0.0, b = 0.0;
	
	float h = H / 360;
	float s = S / 100;
	float v = V / 100;
	
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	
	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}
	
	RGBColour color;
	color.r = r * 255;
	color.g = g * 255;
	color.b = b * 255;
	
	return color;
}
