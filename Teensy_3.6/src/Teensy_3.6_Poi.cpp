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
#define CHANNEL 60 //CHANNEL: 20 30 40 50 60 70 -> POI (1 2 3 4 5 6) 
//Poi 1 - 4 -> Teensy 4.0 -> Channel 20 - 50
//Poi 5 & 6 -> Teensy 3.6 -> Channel 60 & 70

#include <Arduino.h>
#include <SPI.h>
// #include <nRF24L01.h>
#include <RF24.h>
#include "FastLED.h"
#include "C:\Users\felix\Documents\PlatformIO\Projects\Teensy_4.0\src\pic_array.h"
#include <teensy_3.6_hardware.h>
#include "C:\Users\felix\Documents\PlatformIO\Projects\Teensy_4.0\src\variables_receivers.h"
#include "C:\Users\felix\Documents\PlatformIO\Projects\Teensy_4.0\src\visual_functions.h"
#include "C:\Users\felix\Documents\PlatformIO\Projects\Teensy_4.0\src\show_decision.h"
#include "C:\Users\felix\Documents\PlatformIO\Projects\Teensy_4.0\src\setup_serial_radio.h"
//#include "send_voltage.h"



void setup() 
{
  setup_serial_radio();
}




//##########################################################################################
//                                    MAIN LOOP
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
  
  //main function to show pictures
  show_decision(message_global);

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
