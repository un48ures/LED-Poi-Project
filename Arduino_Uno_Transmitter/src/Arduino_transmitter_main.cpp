/*
  Teensy 4.0 - Poi Project
         //Transmitter Signal Strength Test

  by Felix Geisler
  16.03.2022
*/

#include <variables.h>
#include <get_signal_strength.h>
#include <send_data.h>
#include <videolight.h>
#include <midi_mode.h>

//#define PRINT_SIGNAL_STRENGTH
//#define DEBUG_SEND
//#define VIDEO_LIGHT
#define MIDI_MODE

RF24 radio(7, 8); // CE, CSN

void setup()
{
  // Configuration I/O
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(TASTER, INPUT); // Pin 3 button Input

  // Serial configuration
  Serial.begin(115200);

  // Radio configuration
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_1MBPS);
  radio.setRetries(5, 3);
  radio.stopListening();
  radio.openWritingPipe(pipe_address);

  // Set LEDs
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  delay(500);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
}

void loop()
{
#ifdef PRINT_SIGNAL_STRENGTH
  print_signal_strength(&radio, CHs, int(sizeof(CHs)));
#endif

#ifdef VIDEO_LIGHT
  video_light_mode(&radio);
#endif

#ifdef MIDI_MODE
  midi_mode(&radio);
#endif
}