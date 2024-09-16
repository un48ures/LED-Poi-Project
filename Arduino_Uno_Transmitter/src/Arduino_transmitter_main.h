#ifndef ARDUINO_TRANSMITTER_MAIN
#define ARDUINO_TRANSMITTER_MAIN

#include <Arduino.h>

#define NUM_RECEIVERS 6

// Radio configuration settings
const byte pipe_address[6] = "00001";         // Pipe address
const byte CHs[NUM_RECEIVERS] = {10, 20, 30, 40, 50, 60}; // Channels of pois 1-6
const int retries_global = 15;
const int delay_retries_global = 2;

// HW Pins
enum HW_PINS
{
    LED_GREEN = 9,
    LED_RED = 6,
    TASTER = 3
};

typedef struct General_Message{
    int mode = 1;
    int receiver_id;
    int picture;
    int hue;
    int saturation;
    int value_brightness;
    int velocity;
}message;

typedef struct receiver{
    byte receiver_id;
    byte channel;
    float voltage;
    int signalStrength;
}receiver;

void get_serial_message(message *input_message);
void picture_mode();
void battery_signal_strength();

#endif