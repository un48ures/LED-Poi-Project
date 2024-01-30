#include <stdio.h>
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio configuration settings
const byte pipe_address[6] = "00001";         // Pipe address
const byte CHs[6] = {10, 20, 30, 40, 50, 60}; // Channels of pois 1-6

// NoteOn+Kanal, Note, Pitch-Bend cmd, Pitch-Bend val1,val2
int data0 = 0;
byte data_array[5] = {0}; // data array for manuel control via button and poti
byte array0[6][5] = {};

int brightness = 0;
int brightness_old = 0;

// HW Pins
enum HW_PINS
{
    LED_GREEN = 9,
    LED_RED = 6,
    TASTER = 3
};

// State variables
boolean button_state = 0; // init state taster
boolean button_state_old = 0;
boolean taster_change = 0;
byte program = 0;     // Mode/Status Zählervariable
byte program_old = 0; // alter Wert

// Poti
int poti = A0;       // Poti Anschluss A0