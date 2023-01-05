/*
  Teensy 4.0 - Poi Project
         //Transmitter Signal Strength Test

  by Felix Geisler
  16.03.2022
*/

//#define PRINT_SIGNAL_STRENGTH
#define MODE 0  //0 -> PRINT_SIGNAL_STRENGTH
                //1 -> VIDEO_LIGHT
                //2 -> MIDI_MODE


#include <variables_transmitter.h>
#include <signal_strength_test.h>
#include <send_data.h>
#include <videolight.h>
#include <midi_mode.h>



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
  radio.setRetries(4, 2); //One retry as default
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
#if MODE == 0
  print_signal_strength(&radio, CHs, int(sizeof(CHs)));
#endif

#if MODE == 1
  video_light_mode(&radio);
#endif

#if MODE == 2
  midi_mode(&radio, sizeof(CHs));
#endif
}