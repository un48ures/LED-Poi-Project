#include <RF24.h>
#include <Arduino.h>
#include <Arduino_transmitter_main.h>
#include <modes.h>

#define NOTE_ON 144
#define PITCH_BEND 224

// Poti
int poti = A0;       // Poti Anschluss A0

// State variables
boolean button_state = 0; // init state taster
boolean button_state_old = 0;
boolean taster_change = 0;
byte program = 0;     // Mode/Status Zählervariable
byte program_old = 0; // alter Wert

// NoteOn+Kanal, Note, Pitch-Bend cmd, Pitch-Bend val1,val2
int data0 = 0;
byte data_array[5] = {0}; // data array for manuel control via button and poti
byte array0[6][5] = {};

int brightness = 0;
int brightness_old = 0;

void midi_mode(RF24 *radio, int total_number_receivers)
{
    // HAUPTFUNKTION:
    // Das Musik-Programm sendet Töne und deren Pitch-Bend über MIDI/Seriell an den Arduino
    // 144+Kanal = Note-on Befehl gefolgt von einem Byte für die Tonhöhe -> hier: Instrument 1(Kanal 1) für Poi 1, Tonhöhe 60 für Bild 1
    // 224+Kanal = Pitch Bend -> hier: Signal für Helligkeit
    // Serielle Schnittstelle nach neuen Paketen abfragen
    int data0 = 0;
    if (Serial.available() > 1)
    {
        radio->openWritingPipe(pipe_address);
        data0 = Serial.read();
        // Check if first byte is Note On or Pitch Bend
        if (data0 >= NOTE_ON && data0 < (NOTE_ON + total_number_receivers))
        {
            //NOTE ON -> 144 (1st channel); NOTE ON -> 145 (2nd channel)...
            array0[data0 - NOTE_ON][0] = data0;
            array0[data0 - NOTE_ON][1] = Serial.read();
            
            send_data(CHs[data0 - NOTE_ON], array0[data0 - NOTE_ON], sizeof(array0[data0 - NOTE_ON]), pipe_address, radio);
            
            //LED RED blink when data is send
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_RED, LOW);
        }
        else if (data0 >= PITCH_BEND && data0 < (PITCH_BEND + total_number_receivers))
        {
            //PITCH BEND -> 224 (1st channel); PITCH BEND -> 225 (2nd channel)...
            array0[data0 - PITCH_BEND][2] = data0;
            array0[data0 - PITCH_BEND][3] = Serial.read();
            array0[data0 - PITCH_BEND][4] = Serial.read();
            radio->setChannel(CHs[data0 - PITCH_BEND]);
            radio->write(&array0[data0 - PITCH_BEND], sizeof(array0[data0 - PITCH_BEND]));
        }
    }
}

void video_light_mode(RF24* radio)
{
  // Read poti analog input
  brightness = map(analogRead(poti), 0, 1023, 0, 50);

  // Read button input
  button_state = digitalRead(TASTER);

  // Increment program counter
  if (button_state_old == LOW and button_state == HIGH)
  {
    program++;
    button_state_old = HIGH;
  }
  if (button_state_old == HIGH and button_state == LOW)
  {
    button_state_old = LOW;
  }

  // If change then send
  if (brightness_old != brightness || program_old != program)
  {
    brightness_old = brightness;
    program_old = program;

    if (program > 30)
    {
      program = 0;
    }

    Serial.print("Program counter: ");
    Serial.println(program);
  
    // data_array[1] = program;
    // data_array[3] = brightness;

    switch (program)
    {
    case 0:
      for (uint8_t i = 0; i < sizeof(CHs); i++)
      {
        data_array[1] = 0; // Black
        send_data(CHs[i], data_array, (uint8_t) sizeof(data_array), pipe_address, radio);
        data_array[1] = 99; // Blink red 1st LED Pixel
        send_data(CHs[i], data_array, (uint8_t) sizeof(data_array), pipe_address, radio);
      }
      break;

    default:
      for (uint8_t i = 0; i < sizeof(CHs); i++)
      {
        data_array[1] = program;
        data_array[3] = brightness;
        send_data(CHs[i], data_array, (uint8_t) sizeof(data_array), pipe_address, radio);
      }
      break;
    }
  }
}

//void new_remote_control()
//{
//    Serial.print("Test");
//}

/// @brief Send data via the transceiver module
/// @param ch Target channel number
/// @param pipe_address Pipe_Address; usually fixed; theoretically 6 different pipes possible
/// @param content data array with 5 bytes to be send
/// @param size Size of data_array (content) - must be calculated out of this function
void send_data(uint8_t ch, byte *content, uint8_t size, const uint8_t *pipe_address, RF24 *radio)
{
    digitalWrite(LED_RED, HIGH);
    radio->setChannel(ch);
    radio->openWritingPipe(pipe_address);
    int status = radio->write(content, size);
    digitalWrite(LED_RED, LOW);
#define DEBUG_SEND
#ifdef DEBUG_SEND
    Serial.print("Content: ");
    Serial.print(content[0]);
    Serial.print(" ");
    Serial.print(content[1]);
    Serial.print(" ");
    Serial.print(content[2]);
    Serial.print(" ");
    Serial.print(content[3]);
    Serial.print(" ");
    Serial.print(content[4]);
    Serial.print("\t to channel: ");
    Serial.print(ch);
    Serial.print(" via pipe: ");
    Serial.print(pipe_address[0]);
    Serial.print(pipe_address[1]);
    Serial.print(pipe_address[2]);
    Serial.print(pipe_address[3]);
    Serial.print(pipe_address[4]);
    Serial.print(pipe_address[5]);
    Serial.print("\t send status: ");
    Serial.println(status);
    // Serial.print("\t size_1: ");
    // Serial.print(size_1);
    // Serial.print("\t size parameter: ");
    // Serial.print(size);
    // Serial.print("\taddress content: ");
    // Serial.println((unsigned int)content, HEX);
#endif
}

const uint16_t num_test_bytes = 32;
const uint16_t repititions = 100;


/// @brief Sending every poi 100 packets and checking how much got received successfully.
///        Starting at the first element of CHs array for all ch_total count
/// @param radio Radio object used to send/receive data
/// @param CHs Array of all channel numbers
/// @param ch_total Total count of used channels in CHs array
void print_signal_strength(RF24 *radio, const byte *CHs, int8_t ch_total)
{
    static unsigned long old_time = 0;
    if (millis() > (old_time + 3000))
    {
        radio->setRetries(0, 0); // No retries allowed
        for (int j = 0; j < ch_total; j++)
        {
            char buffer[num_test_bytes] = {0};
            int counter = 0;
            radio->setChannel(CHs[j]);

            unsigned long startTime, endTime;
            float speed;
            int signalStrength;
            uint8_t status = 0;
            startTime = millis();
            for (uint8_t i = 0; i < repititions; i++)
            {
                status = radio->write(buffer, sizeof(buffer)); // send x bytes of data. It does not matter what it is
                if (status)
                    counter++;

                // delay(1); // try again in 1 millisecond
            }
            endTime = millis();
            speed = (float) counter * num_test_bytes / (endTime - startTime);
            signalStrength = (float)counter / repititions * 100.0;
            Serial.print("Channel:");
            Serial.print(CHs[j]);
            Serial.print("\t");
            Serial.print("Signal strength: ");
            Serial.print(signalStrength);
            Serial.print(" %\t Speed: ");
            Serial.print(speed);
            Serial.print(" kB/s");
            Serial.print("\t Duration: ");
            Serial.print(endTime - startTime);
            Serial.print(" ms ");
            Serial.print("\t Data: ");
            Serial.print((float)counter * num_test_bytes / 1000.0);
            Serial.println(" kB");
        }
        Serial.println(" ");
        old_time = millis();
    }
    // _delay_ms(500);
}