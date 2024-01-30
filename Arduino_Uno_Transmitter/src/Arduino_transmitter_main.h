#ifndef ARDUINO_TRANSMITTER_MAIN
#define ARDUINO_TRANSMITTER_MAIN

#include <Arduino.h>

// Radio configuration settings
const byte pipe_address[6] = "00001";         // Pipe address
const byte CHs[6] = {10, 20, 30, 40, 50, 60}; // Channels of pois 1-6

// HW Pins
enum HW_PINS
{
    LED_GREEN = 9,
    LED_RED = 6,
    TASTER = 3
};

typedef struct RF_Message{
    byte mode;
    byte picture_hue;
    byte saturation;
    byte value_brightness;
    byte velocity;
}RF_Message;

#endif