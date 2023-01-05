/*
  Teensy 4.0 - Poi Project
         - Receiver Code

  by Felix Geisler
  16.03.2022

  *This sketch outputs images to persistence of vision led strips
  *It uses FastLed to drive APA102 leds, sending colour values from
  *arrays held in flash memory (designated by 'const'). You need to
  *set the number of slices you have made your image into,
  *e.g. bmp image of 61 pixels high by 150 wide
  * would give 60 num_leds and
  * 150 slices (number of slices you have made your image into)
*/

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "FastLED.h"
#include "pic_array.cpp"
#include <teensy_4.0_hardware.h>
#include <variables_receivers.h>
#include <visual_functions.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  SPI.setSCK(RF24_SCK);
  SPI.begin();
  delay(200);

  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(10)>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);

  radio.begin();
  radio.openReadingPipe(0, pipe_address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  // radio.setRetries(0,15);
  radio.setChannel(CHANNEL);
  // radio.enableDynamicPayloads() ;
  // radio.setAutoAck( true ) ;
  // radio.powerUp() ;
  radio.startListening();

  // pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, HIGH);
  Serial.println("Initializing done");
}


/// @brief Main Loop
void loop()
{
  // Testlight when first start
  if (firstStart)
  {
    StartDemo();
    firstStart = false;
  }

  // Reset for Fillup and Filldown function if another pic was shown
  if (message_global != 8 || message_global != 9)
  {
    fillupDone = 0;
    filldownDone = 0;
  }

  // Begin showing graphics
  switch (message_global)
  {
  case 0:
    LED_fill_black();
    FastLED.show();
    break;
  case 1:
    LED_fill_white();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 2:
    LED_fill_red();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 3:
    LED_fill_green();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 4:
    LED_fill_blue();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 5:
    LED_fill_purple();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 6:
    LED_fill_yellow();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 7:
    // FADE PULSE - BLUE
    hue = 150; // BLUE
    LED_Pulsing(hue, message_brightness);
    delay(50);
    break;
  case 8:
    // FADE PULSE - GREEN
    hue = 100; // GREEN
    LED_Pulsing(hue, message_brightness);
    delay(50);
    break;
  case 9:
    // FADE PULSE - RED
    hue = 255; // GREEN
    LED_Pulsing(hue, message_brightness);
    delay(50);
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
    rainbow();
    break;
  case 11:
    LED_strobo();
    break;
  case 12:
    PoiSonic(50, array1);
    break;
  case 13:
    PoiSonic(50, array2);
    break;
  case 14:
    PoiSonic(50, array3);
    break;
  case 15:
    PoiSonic(50, array4);
    break;
  case 16:
    PoiSonic(50, array5);
    break;
  case 17:
    PoiSonic(50, array6);
    break;
  case 18:
    PoiSonic(50, array7);
    break;
  case 19:
    PoiSonic(50, array8);
    break;
  case 20:
    PoiSonic(50, array9);
    break;
  case 21:
    PoiSonic(50, array10);
    break;
  case 22:
    PoiSonic(50, array11);
    break;
  case 23:
    PoiSonic(50, array12);
    break;
  case 24:
    PoiSonic(50, array13);
    break;
  case 25:
    PoiSonic(50, array14);
    break;
  case 26:
    PoiSonic(50, array15);
    break;
  case 27:
    PoiSonic(50, array16);
    break;
  case 28:
    PoiSonic(50, array17);
    break;
  case 29:
    PoiSonic(50, array18);
    break;
  case 30:
    PoiSonic(50, array19);
    break;
  case 31:
    PoiSonic(50, array20);
    break;
  case 32:
    PoiSonic(50, array21);
    break;

  default:
    LED_blink_red();
    break;
  }

  // Data Receive
  if (radio.available())
  {
    byte array1[5];
    radio.read(&array1, sizeof(array1));
    message_global = array1[1];
    message_brightness = array1[3];
    printf("%i\n%i\n%i\n%i\n%i\n",array1[0], array1[1],array1[2],array1[3],array1[4],array1[5]);
    printf("Message Brightness: %i \n", message_brightness);
    printf("Current Millis: %i\n", millis());
  }
}
