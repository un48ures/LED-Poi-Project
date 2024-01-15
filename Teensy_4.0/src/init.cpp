// #include <FastLED.h>
#include <Arduino.h>
#include <RF24.h>
// #include <nRF24L01.h>
#include "teensy_4.0_hardware.h"
#include "init.h"

void setup_Serial()
{
    Serial.begin(115200);
}

void setup_SPI()
{
    SPI.setSCK(RF24_SCK);
    SPI.begin();
}

void setup_LED()
{
    // leds[0] = CRGB::Black;
    // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(10)>(leds, NUM_LEDS);
    // FastLED.clear();
    // FastLED.setBrightness(INIT_BRIGHTNESS);
}

void setup_Radio()
{
    radio.begin();
    radio.openReadingPipe(0, pipe_address);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_1MBPS);
    // radio.setRetries(0,15);
    radio.setChannel(INIT_CHANNEL);
    // radio.enableDynamicPayloads() ;
    // radio.setAutoAck( true ) ;
    // radio.powerUp() ;
    radio.startListening();
}