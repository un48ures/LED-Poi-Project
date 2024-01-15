#ifndef INIT_CONFIG
#define INIT_CONFIG

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
// #include <nRF24L01.h>
#include "teensy_4.0_hardware.h"
// #include <FastLED.h>

#define INIT_BRIGHTNESS 2
#define INIT_CHANNEL 20

uint64_t pipe_address = 1;
RF24 radio(RF24_CE, RF24_CSN);

void setup_Serial();
void setup_SPI();
void setup_LED();
void setup_Radio();

#endif