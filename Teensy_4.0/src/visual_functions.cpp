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
bool dim_up_done = false;
bool picture_changed = false;
unsigned long starttime = 0;
int pics_start_nr = 20; // the first poi array (picture) shows at msg.picture = 20

const unsigned int *pic_array[31] = {array1, array2, array3, array4, array5, array6, array7, array8, array9, array10, array11, array12, array13, array14, 
                                    array15, array16, array17, array18, array19, array20, array21, array22, array23, array24, array25, array26, array27, array28, 
                                    array29, array30, array31};
int pic_array_length[31] = {sizeof(array1), sizeof(array2), sizeof(array3), sizeof(array4), sizeof(array5), sizeof(array6), sizeof(array7), sizeof(array8), 
                              sizeof(array9), sizeof(array10), sizeof(array11), sizeof(array12), sizeof(array13), sizeof(array14), sizeof(array15), sizeof(array16), 
                              sizeof(array17), sizeof(array18), sizeof(array19), sizeof(array20), sizeof(array21), sizeof(array22), sizeof(array23), sizeof(array24), 
                              sizeof(array25), sizeof(array26), sizeof(array27), sizeof(array28), sizeof(array29), sizeof(array30), sizeof(array31)};

/// @brief Make decision which array to show in relation to the received message_global
/// @param message_global  contains data which picture/array should be shown
void display(message msg, CRGB *leds)
{
  static int old_picture = 0;
  picture_changed = false;

  if (msg.picture != old_picture)
  {
    picture_changed = true;
    fillupDone = false;
    dim_up_done = false;
  }

  if ((msg.picture >= pics_start_nr) && (((unsigned int)msg.picture - pics_start_nr + 1) <= (sizeof(pic_array_length) / sizeof(int))))
  {
    showPicOnce(pic_array[msg.picture - pics_start_nr], pic_array_length[msg.picture - pics_start_nr], msg, leds);
  }

  switch (msg.picture)
  {
  case 0: // BLACK
    LED_show_color(Black, msg.value_brightness, leds);
    break;
  case 1: // WHITE
    LED_show_color(White, msg.value_brightness, leds);
    break;
  case 2: // PULSE
    LED_Pulsing(msg.hue, msg.value_brightness, msg.velocity, leds);
    break;
  case 3: // STROBO
    LED_strobo(msg.value_brightness, msg.velocity, leds);
    break;
  case 4: // RAINBOW
    FastLED.setBrightness(msg.value_brightness);
    rainbow(msg, leds);
    break;
  case 5: // confetti
    confetti(msg, leds);
    break;
  case 6: // sinelon
    sinelon(msg, leds);
    break;
  case 7: // bpm
    bpm(msg, leds);
    break;
  case 8: // juggle
    juggle(msg, leds);
    break;
  case 9: // FILL UP
    LED_fillup(msg, false, leds);
    break;
  case 10: // FILL DOWN (fill up reversed)
    LED_fillup(msg, true, leds);
    break;
  case 11: // DIM UP
    dim_up(msg, leds);
    break;
  case 12: // DIM DOWN
    dim_down(msg, leds);
    break;
  case 13: // RUN UP
    LED_runner(msg, false, leds);
    break;
  case 14: // RUN DOWN
    LED_runner(msg, true, leds);
    break;

  default:
    LED_blink_red(leds);
    break;
  }
  old_picture = msg.picture;
}

void showPicOnce(const unsigned int array[], int array_length, message msg, CRGB *leds)
{
  int num_slices = array_length / 61 / 4; // 4 Byte per pixel
  for (int x = 0; x < num_slices; x++)
  {
    for (int z = NUM_LEDS; z > 0; z--)
    {
      leds[z - 1] = array[x + ((NUM_LEDS - z) * num_slices)];
    }
    FastLED.setBrightness(msg.value_brightness);
    FastLED.show();
    delayMicroseconds(msg.velocity * 25); // Pause between slices - may need to increase / decrease depending on spin rate original value = 40
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
  if(picture_changed)
  {
    LED_show_color(Black, DEFAULT_BRIGHTNESS, leds);
  }

  unsigned long currentMillis = millis(); // for LED blink

  if ((currentMillis - previousMillis) >= interval_1)
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
      fill_solid(leds, NUM_LEDS, CRGB::Black);
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
        fill_solid(leds, NUM_LEDS, CRGB::Black);
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
// FILL FROM BOTTOM OR TOP (Reverse)
//-------------------------------------------------------------------------------------------------------------
void LED_fillup(message msg, bool reverse, CRGB *leds)
{
  static int i = 0;
  if(picture_changed)
  {
    starttime = millis();
    i = 0;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }

  if (fillupDone == false)
  {
    RGBColour c = hsv2rgb(msg.hue / 255.0 * 360.0, 100, 100); // transform 255 to 360° hue, ignore saturation and brightness

    if ((i < NUM_LEDS) && (millis() >= (starttime + msg.velocity)))
    {
      if(!reverse)
      {
        leds[i].r = c.r;
        leds[i].g = c.g;
        leds[i].b = c.b;
      }
      else
      {
        leds[NUM_LEDS - i - 1].r = c.r;
        leds[NUM_LEDS - i - 1].g = c.g;
        leds[NUM_LEDS - i - 1].b = c.b;
      }
      FastLED.setBrightness(msg.value_brightness);
      FastLED.show();
      starttime = millis();
      i++;
    }
    else if ((i >= NUM_LEDS) && (millis() > (starttime + msg.velocity)))
    {
      fillupDone = true;
    }
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

void rainbow(message msg, CRGB *leds)
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
  addGlitter(80, leds);
  FastLED.setBrightness(msg.value_brightness);
  FastLED.show();
  // FastLED.delay(50); !!!!
  EVERY_N_MILLISECONDS(msg.velocity)
  {
    gHue++;
  }
}

void confetti(message msg, CRGB *leds) 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  FastLED.setBrightness(msg.value_brightness);
  FastLED.show();
}

void sinelon(message msg, CRGB *leds)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( msg.velocity, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
  FastLED.setBrightness(msg.value_brightness);
  FastLED.show();
}

void bpm(message msg, CRGB *leds)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  FastLED.setBrightness(msg.value_brightness);
  FastLED.show();
}

void juggle(message msg, CRGB *leds) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.setBrightness(msg.value_brightness);
  FastLED.show();
}

void dim_down(message msg, CRGB *leds)
{
  if(picture_changed)
  {
    starttime = millis();
  }
  if (millis() >= (starttime + msg.velocity))
  {
    fadeToBlackBy(leds, NUM_LEDS, 50);
    FastLED.setBrightness(msg.value_brightness);
    FastLED.show();
    starttime = millis();
  }
}

void dim_up(message msg, CRGB *leds) // need implementation
{
  static int i = 0;
  if(picture_changed)
  {
    // printf("Picture changed - begin dim_up\n");
    starttime = millis();
    i = 0;
    RGBColour c = hsv2rgb(msg.hue / 255.0 * 360.0, 100, 100); // transform 255 to 360° hue, ignore saturation and brightness
    for(int j = 0; j <= NUM_LEDS; j++)
    {
      leds[j].r = c.r;
      leds[j].g = c.g;
      leds[j].b = c.b;
    }
  }
  if (dim_up_done == false)
  {
    // printf("dim_up_done == false\n");
    if((millis() > (starttime + msg.velocity)))
    {
      FastLED.setBrightness(i);
      FastLED.show();
      i++;
      starttime = millis();
      // printf("i = %d and new starttime = %d\n", i, starttime);
    }
    if(i >= msg.value_brightness)
    {
      dim_up_done = true;
      //printf("Dim up done!\n");
    }
  }
}

//-------------------------------------------------------------------------------------------------------------
// RUN LIGHT POINT FROM BOTTOM TO TOP (or reverse)
//-------------------------------------------------------------------------------------------------------------
void LED_runner(message msg, bool reverse, CRGB *leds)
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  LED_fillup(msg, reverse, leds);
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
