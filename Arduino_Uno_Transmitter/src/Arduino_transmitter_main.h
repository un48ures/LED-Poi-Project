#ifndef ARDUINO_TRANSMITTER_MAIN
#define ARDUINO_TRANSMITTER_MAIN

#include <Arduino.h>

#define NUM_RECEIVERS 6

// Radio configuration settings
const byte pipe_address[6] = "00001";         // Pipe address
const byte CHs[NUM_RECEIVERS] = {10, 20, 30, 40, 50, 60}; // Channels of pois 1-6

// HW Pins
enum HW_PINS
{
    LED_GREEN = 9,
    LED_RED = 6,
    TASTER = 3
};

typedef struct General_Message{
    byte mode;
    byte receiver_id;
    byte picture_hue;
    byte saturation;
    byte value_brightness;
    byte velocity;
}message;

typedef struct receiver{
    byte receiver_id;
    byte channel;
    float voltage;
    int signalStrength;
}receiver;

void get_serial_message(message *input_message);

#endif