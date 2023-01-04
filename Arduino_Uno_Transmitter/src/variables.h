#include <stdio.h>
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio configuration settings
const byte pipe_address[6] = "00001";         // Pipe address
const byte CHs[6] = {20, 30, 40, 50, 60, 70}; // Channels of pois 1-6

// NoteOn+Kanal, Note, Pitch-Bend cmd, Pitch-Bend val1,val2
int data0 = 0;
byte array_manual_ctrl[5] = {0, 0, 0, 0, 0}; // data array for manuel control via button and poti
byte array1[5] = {144, 0, 0, 0, 0};
byte array2[5] = {145, 0, 0, 0, 0};
byte array3[5] = {146, 0, 0, 0, 0};
byte array4[5] = {147, 0, 0, 0, 0};
byte array5[5] = {148, 0, 0, 0, 0};
byte array6[5] = {149, 0, 0, 0, 0};

int brightness = 0;
int brightness_old = 0;

// HW Pins
int LED_green = 9; // Pin 9
int LED_red = 6;   // Pin 6
int taster = 3;    // Pin 3

// State variables
int tasterstatus = 0; // init state taster
int tasterstatus_old = 0;
int taster_change = 0;
byte taster_z = 0;     // Mode/Status Zählervariable
byte taster_z_old = 0; // alter Wert

// Poti
int eingang = A0;       // Poti Anschluss A0
int sensorwert = 0;     // init poti value
int sensorwert_old = 0; // alter Wert