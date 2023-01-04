#define NOTE_ON 144
#define PITCH_BEND 224

void midi_mode(RF24 *radio)
{
    // HAUPTFUNKTION:
    // Das Musik-Programm sendet Töne und deren Pitch-Bend über MIDI/Seriell an den Arduino
    // 144+Kanal = Note-on Befehl gefolgt von einem Byte für die Tonhöhe -> hier: Instrument 1(Kanal 1) für Poi 1, Tonhöhe 60 für Bild 1
    // 224+Kanal = Pitch Bend -> hier: Signal für Helligkeit
    // Serielle Schnittstelle nach neuen Paketen abfragen
    if (Serial.available() > 1)
    {
        digitalWrite(LED_GREEN, HIGH);
        radio->openWritingPipe(pipe_address);
        data0 = Serial.read();
        // Check if first
        if (data0 == NOTE_ON)
        {
            array0[data0 - NOTE_ON][1] = data0;
            array0[data0 - NOTE_ON][2] = Serial.read();
            radio->setChannel(CHs [data0 - NOTE_ON]);
            radio->write(&array0[data0 - NOTE_ON], sizeof(array0[data0 - NOTE_ON]));
        }
        else if(data0 == PITCH_BEND)
        {
            array0[data0 - PITCH_BEND][2] = data0;
            array0[data0 - PITCH_BEND][3] = Serial.read();
            array0[data0 - PITCH_BEND][4] = Serial.read();
            radio->setChannel(CHs[data0 - PITCH_BEND]);
            radio->write(&array0[data0 - PITCH_BEND], sizeof(array0[data0 - PITCH_BEND]));
        }
        digitalWrite(LED_RED, HIGH);
        long time = millis();
        if ((millis() - time) > 25)
        {
            digitalWrite(LED_RED, LOW);
        }
    }
}

    // // Instrument_1-----------------------------------------------------
    // if (data0 == 144)
    // {
    //     array1[0] = 144;
    //     array1[1] = Serial.read();
    //     // Ch10
    //     digitalWrite(LED_red, HIGH);
    //     radio.setChannel(CH_P1);
    //     radio.openWritingPipe(pipe_address);
    //     radio.write(&array1, sizeof(array1));
    //     delay(25);
    //     digitalWrite(LED_red, LOW);
    // }

    // if (data0 == 224)
    // {
    //     array1[2] = 224;
    //     array1[3] = Serial.read();
    //     array1[4] = Serial.read();
    // }

    // // Instrument_2-----------------------------------------------------
    // if (data0 == 145)
    // {
    //     array2[0] = 145;
    //     array2[1] = Serial.read();
    //     digitalWrite(LED_red, HIGH);
    //     radio.setChannel(CH_P2);
    //     radio.openWritingPipe(pipe_address);
    //     radio.write(&array2, sizeof(array2));
    //     delay(25);
    //     digitalWrite(LED_red, LOW);
    // }

    // if (data0 == 225)
    // {
    //     array2[2] = 225;
    //     array2[3] = Serial.read();
    //     array2[4] = Serial.read();
    // }

    // // Instrument_3-----------------------------------------------------
    // if (data0 == 146)
    // {
    //     array3[0] = 146;
    //     array3[1] = Serial.read();
    //     digitalWrite(LED_red, HIGH);
    //     radio.setChannel(CH_P3);
    //     radio.openWritingPipe(pipe_address);
    //     radio.write(&array3, sizeof(array3));
    //     delay(25);
    //     digitalWrite(LED_red, LOW);
    // }

    // if (data0 == 226)
    // {
    //     array3[2] = 226;
    //     array3[3] = Serial.read();
    //     array3[4] = Serial.read();
    // }

    // // Instrument_4-----------------------------------------------------
    // if (data0 == 147)
    // {
    //     array4[0] = 147;
    //     array4[1] = Serial.read();
    //     digitalWrite(LED_red, HIGH);
    //     radio.setChannel(CH_P4);
    //     radio.openWritingPipe(pipe_address);
    //     radio.write(&array4, sizeof(array4));
    //     delay(25);
    //     digitalWrite(LED_red, LOW);
    // }

    // if (data0 == 227)
    // {
    //     array4[2] = 227;
    //     array4[3] = Serial.read();
    //     array4[4] = Serial.read();
    // }

    // // Instrument_5-----------------------------------------------------
    // if (data0 == 148)
    // {
    //     array5[0] = 148;
    //     array5[1] = Serial.read();
    //     digitalWrite(LED_red, HIGH);
    //     radio.setChannel(CH_P5);
    //     radio.openWritingPipe(pipe_address);
    //     radio.write(&array5, sizeof(array5));
    //     delay(25);
    //     digitalWrite(LED_red, LOW);
    // }

    // if (data0 == 228)
    // {
    //     array5[2] = 228;
    //     array5[3] = Serial.read();
    //     array5[4] = Serial.read();
    // }

    // // Instrument_6-----------------------------------------------------
    // if (data0 == 149)
    // {
    //     array6[0] = 149;
    //     array6[1] = Serial.read();
    //     digitalWrite(LED_red, HIGH);
    //     radio.setChannel(CH_P6);
    //     radio.openWritingPipe(pipe_address);
    //     radio.write(&array6, sizeof(array6));
    //     delay(25);
    //     digitalWrite(LED_red, LOW);
    // }

    // if (data0 == 229)
    // {
    //     array6[2] = 229;
    //     array6[3] = Serial.read();
    //     array6[4] = Serial.read();
    // }
// }
// }