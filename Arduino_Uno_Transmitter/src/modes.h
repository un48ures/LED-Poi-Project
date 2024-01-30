#define NOTE_ON 144
#define PITCH_BEND 224

void midi_mode(RF24 *radio, int total_number_receivers)
{
    // HAUPTFUNKTION:
    // Das Musik-Programm sendet Töne und deren Pitch-Bend über MIDI/Seriell an den Arduino
    // 144+Kanal = Note-on Befehl gefolgt von einem Byte für die Tonhöhe -> hier: Instrument 1(Kanal 1) für Poi 1, Tonhöhe 60 für Bild 1
    // 224+Kanal = Pitch Bend -> hier: Signal für Helligkeit
    // Serielle Schnittstelle nach neuen Paketen abfragen
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

void new_remote_control()
{
    ;
}