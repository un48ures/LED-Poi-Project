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
                //3 -> SINGAL_STRENGTH + VIDEO_LIGHT
                //4 -> SERIAL ECHO
                //5 -> NEW REMOTE CONTROL MODE


#include <Arduino_transmitter_main.h>
#include <modes.h>

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
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS); //250kbs makes problems
  radio.setRetries(1, 1); //One retry as default after 250us
  radio.stopListening();
  radio.setChannel(10); //predefine default Channel
  // radio.enableAckPayload(); // request Acknoledge Payload from the receivers
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

#if MODE == 3
  print_signal_strength(&radio, CHs, int(sizeof(CHs)));
  video_light_mode(&radio);
#endif

#if MODE == 4
  if(Serial.available() > 0)
  {
    int input = Serial.read();
      //ECHO
    Serial.print("ECHO: ");
    Serial.println(input);
  }
#endif

#if MODE == 5
  new_remote_control();
#endif
}
