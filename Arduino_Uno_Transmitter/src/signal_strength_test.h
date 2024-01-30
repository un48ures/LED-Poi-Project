
#include <RF24.h>
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