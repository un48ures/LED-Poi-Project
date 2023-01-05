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

//NRF240 HW Pins - Radio settings
#define RF24_MOSI 11
#define RF24_MISO 12
#define RF24_SCK 13
#define CHANNEL 20 //CHANNEL: 20 30 40 50 60 70 -> POI (1 2 3 4 5 6) 
//Poi 1 - 4 -> Teensy 4.0 -> Channel 20 - 50
//Poi 5 & 6 -> Teensy 3.6 -> Channel 60 & 70
RF24 radio(4, 5); // CE, CSN
const byte pipe_address[6] = "00001";

//FastLED HW Pins
#define NUM_LEDS   61 //number of leds in strip length on one side
#define DATA_PIN   26  //7 = second hardware spi data
#define CLOCK_PIN  27 //14 = second hardware spi clock
#define COLOR_ORDER BGR

//Constants
const int DEFAULT_BRIGHTNESS = 2; //General fix brightness - max brightness 255
const long interval_1 = 1000; //ms 1st LED Standby blink interval
const long LED_red_on_time = 50; //ms 1st LED Standby blink on-time

//LED Variables
CRGB leds[NUM_LEDS];
int numberOfSlices = 150;
int ledState = LOW;
volatile byte fadectr = 0;
String color = "red";
uint8_t gHue = 128;


//Message Variables
int message_global = 99; //default message 99 -> Standby RED LED blinking
long message_brightness = 0;

//Additional
int looping = 0;
bool fillupDone = 0;
bool filldownDone = 0;
unsigned long previousMillis = 0;
//int ledPin = 13; = SCK PIN bei Teensy 4.0
bool firstStart = true;
int LEDstate = 0;
bool blink_once = 0;
int time_on = 0; //help variable for blinking

//PULSE FADE Variablen
int hue = 0; //Color
int saturation = 0; //saturation
int value = 0; //brightness value



void setup() {
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
  radio.setDataRate(RF24_1MBPS) ;
  //radio.setRetries(0,15);
  radio.setChannel(CHANNEL);
  //radio.enableDynamicPayloads() ;
  //radio.setAutoAck( true ) ;
  //radio.powerUp() ;
  radio.startListening();

  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, HIGH);
  Serial.println("Initializing done");
}


//Function to show pictures out of arrays
void PoiSonic(unsigned long time, const unsigned int array[]) {
  unsigned long currentTime = millis();

  while (millis() < currentTime + (time)) {
    int f = numberOfSlices;
    int z; //a counter
    int j = NUM_LEDS;
    for (int x = 0; x < f; x++) {
      for (z = NUM_LEDS; z > 0; z--) {
        leds[z - 1] = array[x + ((j - z) * f)];
      }
      FastLED.setBrightness(message_brightness);
      FastLED.show();
      //Serial.println("FASTLED show...");
      delayMicroseconds(500); //may need to increase / decrease depending on spin rate original value = 40
      //hier
    }
    delayMicroseconds(1000); //Abstand zwischen Bilder - may need to increase / decrease depending on spin rate
  }

}

// Move a single white dot up and down at startup
void StartDemo() {
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::White;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(7);

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;
  }

  for (int whiteLed = NUM_LEDS; whiteLed > 0; whiteLed--) {
    // Turn our current led on to white, then show the leds
    leds[whiteLed] = CRGB::White;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(7);

    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Black;
  }
}

//prototype declaration
void LED_fill_black();

//Standby 1st LED Blink RED
void LED_blink_red() {
  unsigned long currentMillis = millis(); //for LED blink

  if (currentMillis - previousMillis >= interval_1) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    blink_once = 1; //start one blink
  }

  if (blink_once) {
    // if the LED is off turn it on and vice-versa:
    if (LEDstate == 0) {
      //Let the first LED blink red
      leds[0] = CRGB::Red;
      FastLED.setBrightness(DEFAULT_BRIGHTNESS);
      FastLED.show();
      time_on = millis();
      LEDstate = 1;
    }
    if (LEDstate == 1) {
      if (millis() - time_on > LED_red_on_time) {
        leds[0] = CRGB::Black;
        //        LED_fill_black();
        FastLED.show();
        LEDstate = 0;
        blink_once = 0;
      }
    }
  }
}

//FILL COLORS------------------------------------------------------------------------------------------------
void LED_fill_black() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}

void LED_fill_white() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
}

void LED_fill_red() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
}

void LED_fill_green() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;
  }
}

void LED_fill_blue() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
  }
}

void LED_fill_purple() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Purple;
  }
}

void LED_fill_yellow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Yellow;
  }
}

//-------------------------------------------------------------------------------------------------------------
//FADE PULSING
//-------------------------------------------------------------------------------------------------------------
void LED_Pulsing(int hue, int message_brightness) {
  //aufsteigend
  if (looping == 0) {
    for (int j = 0; j < message_brightness; j++) {
      //    if (radio.available()) {last_one = 1;}
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setHSV(hue, 255, 255);
        FastLED.setBrightness(j);
        FastLED.show();
      }
    }
    looping = 1; //jetzt absteigend
  }

  //absteigend
  if (looping == 1) {
    for (int j = message_brightness; j > 0; j--) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setHSV(hue, 255, 255);
        FastLED.setBrightness(j);
        FastLED.show();
      }
    }
    looping = 0;
    LED_fill_black();
    FastLED.show();
  }
}

//-------------------------------------------------------------------------------------------------------------
//FILL STROBO
//-------------------------------------------------------------------------------------------------------------
void LED_strobo() {
  LED_fill_white();
  FastLED.setBrightness(message_brightness);
  FastLED.show();
  delay(50);
  LED_fill_black();
  FastLED.setBrightness(1);
  FastLED.show();
  delay(50);
}



//-------------------------------------------------------------------------------------------------------------
//FILL FROM BOTTOM
//-------------------------------------------------------------------------------------------------------------
void LED_fillup() {
  if (fillupDone == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
      FastLED.show();
      FastLED.delay(25);
    }
    fillupDone = 1;
  }
}

//-------------------------------------------------------------------------------------------------------------
//FILL FROM TOP
//-------------------------------------------------------------------------------------------------------------
void LED_filldown() {
  if (filldownDone == 0) {
    for (int i = NUM_LEDS - 1; i > 0; i--) {
      leds[i] = CRGB::Red;
      FastLED.show();
      FastLED.delay(25);
    }
    filldownDone = 1;
  }
}

//-------------------------------------------------------------------------------------------------------------
//RAINBOW
//-------------------------------------------------------------------------------------------------------------
void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  addGlitter(80);
  FastLED.show();
  FastLED.delay(50);
  EVERY_N_MILLISECONDS( 10 ) {
    gHue++;
  }
}

void fadetoblack()
{
  fadeToBlackBy( leds, NUM_LEDS, 50);
  FastLED.show();
  FastLED.delay(15);
}



//##########################################################################################
//LOOP
//##########################################################################################
void loop() {
  //Testlight when first start
  if (firstStart) {
    StartDemo();
    firstStart = false;
  }

  //Reset for Fillup and Filldown function if another pic was shown
  if (message_global != 8 || message_global != 9) {
    fillupDone = 0;
    filldownDone = 0;
  }

  //Begin showing graphics
  switch (message_global) {
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
      //FADE PULSE - BLUE
      hue = 150; //BLUE
      LED_Pulsing(hue, message_brightness);
      delay(50);
      break;
    case 8:
      //FADE PULSE - GREEN
      hue = 100; //GREEN
      LED_Pulsing(hue, message_brightness);
      delay(50);
      break;
    case 9:
      //FADE PULSE - RED
      hue = 255; //GREEN
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



    case 99:
      LED_blink_red();
      break;

    default:
      LED_blink_red();
      //Serial.println("default case");
      break;
  }

//Data Receive
  if (radio.available()) {
    byte array1[5];
    radio.read(&array1, sizeof(array1));
    message_global = array1[1];
    message_brightness = array1[3];
    Serial.println(array1[0]);
    Serial.println(array1[1]);
    Serial.println(array1[2]);
    Serial.println(array1[3]);
    Serial.println(array1[4]);
    printf("Message Brightness: %i \n", message_brightness);
    printf("Current Millis: %i\n", millis());
  }
  //digitalWrite(ledPin, LOW);
}
