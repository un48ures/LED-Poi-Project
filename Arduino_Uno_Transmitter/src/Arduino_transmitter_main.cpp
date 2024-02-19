/*
  Teensy 4.0 - Poi Project
         //Transmitter Signal Strength Test

  by Felix Geisler
  16.03.2022
*/
#include <Arduino_transmitter_main.h>
#include <modes.h>
#include <LibPrintf.h>

RF24 radio(7, 8); // CE, CSN
int mode = 0;
message message_from_pc;
receiver teensy[NUM_RECEIVERS];

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
  radio.enableAckPayload(); // request Acknoledge Payload from the receivers
  radio.openWritingPipe(pipe_address);

  // Set LEDs
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  delay(500);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);

  for(int i=0; i<NUM_RECEIVERS;i++)
  {
      teensy[i].channel = CHs[i];
  }
   
}

void loop()
{
  // Read incoming serial message from PC and save it in
  // get_serial_message(&message_from_pc);
  // mode = message_from_pc.mode;

  switch (mode){
     case 0:
      print_signal_strength(&radio, teensy, NUM_RECEIVERS); // send x empty bytes and count ack
      break;

    case 1:
      video_light_mode(&radio, teensy); // control manually - change colors and pictures
      break;

    case 2:
      midi_mode(&radio, sizeof(CHs), teensy); // legacy mode to control via MIDI bridge and Studio One Piano
      break;                          // not working here

    case 3:
      if(Serial.available() > 0)  // debug mode - print incoming messages over serial
      {
        int input = Serial.read();
          //ECHO
        Serial.print("ECHO: ");
        Serial.println(input);
      }
      break;

    case 4:
      new_remote_control(); // new control protocol for teensys
      break;

    default:
    break;
  }
}
