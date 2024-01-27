#ifndef HARDWARE
#define HARDWARE

#include <FastLED.h>

#define CHANNEL 23 // CHANNEL: 20 30 40 50 60 70 -> POI (1 2 3 4 5 6)
// Poi 1 - 4 -> Teensy 4.0 -> Channel 20 - 50
// Poi 5 & 6 -> Teensy 3.6 -> Channel 60 & 70

//NRF240 HW Pins - Radio settings
#define RF24_MOSI 11
#define RF24_MISO 12
#define RF24_SCK 13 //Clock
#define RF24_CE 4 //Chip Enable
#define RF24_CSN 5 //Chip Select

//FastLED HW Pins
#define NUM_LEDS   61 //number of leds in strip length on one side
#define DATA_PIN   26  //7 = second hardware spi data
#define CLOCK_PIN  27 //14 = second hardware spi clock
#define COLOR_ORDER BGR
#define DEFAULT_BRIGHTNESS 2

#endif