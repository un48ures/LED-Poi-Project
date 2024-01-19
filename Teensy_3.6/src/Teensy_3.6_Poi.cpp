/*
  Teensy 3.6 - Poi Project
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
// #include <nRF24L01.h>
#include <RF24.h>
#include "FastLED.h"
#include <teensy_3.6_config.h>
#include "C:\Users\felix\Documents\PlatformIO\Projects\Teensy_4.0\src\visual_functions.h"
#include "C:\Users\felix\Documents\PlatformIO\Projects\Teensy_4.0\src\visual_functions.cpp"

//#include "send_voltage.h"

#define PIC_IS_SHOWN 25

const byte pipe_address[6] = "00001";
RF24 radio(RF24_CE, RF24_CSN);
CRGB leds[NUM_LEDS];

// Message Variables
int message_global = 99; // default message 99 -> Standby RED LED blinking
int message_brightness = 0;
int fillupDone = 0;
int filldownDone = 0;
bool firstStart = true;
unsigned int old_time = 0;

void setup() 
{
  Serial.begin(115200);                     //Serial
  printf("Initializing...");    
  SPI.setSCK(RF24_SCK);                     //SPI
  SPI.begin();
  delay(200);                               //LED
  FastLED.addLeds<APA102,(uint8_t) DATA_PIN,(uint8_t) CLOCK_PIN, (EOrder) COLOR_ORDER, (uint32_t) DATA_RATE_MHZ(10)>(leds, (int) NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  radio.begin();                            //radio
  radio.openReadingPipe(0, pipe_address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(CHANNEL);
  radio.startListening();
}




//##########################################################################################
//                                    MAIN LOOP
//##########################################################################################
void loop() {
  //Testlight when first start
  if (firstStart) {
    StartDemo(DEFAULT_BRIGHTNESS, leds);
    firstStart = false;
  }

  //Reset for Fillup and Filldown function if another pic was shown
  if (message_global != 8 || message_global != 9) {
    fillupDone = 0;
    filldownDone = 0;
  }

  //Data Receive
  if (radio.available()) 
  {
    byte array1[5];
    radio.read(&array1, sizeof(array1));
    message_global = array1[1];
    message_brightness = array1[3];
    printf("%i\n%i\n%i\n%i\n%i\n", array1[0], array1[1], array1[2], array1[3], array1[4]);
    printf("Message Brightness: %i \n", message_brightness);
    printf("Current Millis: %i\n", millis());
  }

  // Show pictures
  display(message_global, message_brightness, leds);

  //keep alive message
  if(millis() > old_time + 3000)
  {
    printf("Teensy 4.0 alive - time: %d\n", millis());
    printf("CHANNEL: %d\n", CHANNEL);
    old_time = millis();
  }
}
