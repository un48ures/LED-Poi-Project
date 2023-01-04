
#include <RF24.h>

/// @brief Sending every poi 100 packets and checking how much got received successfully.
///        Starting at the first element of CHs array for all ch_total count
/// @param radio Radio object used to send/receive data
/// @param CHs Array of all channel numbers
void print_signal_strength(RF24 *radio, const byte *CHs, int8_t ch_total)
{
    radio->setRetries(0, 0); //No retries allowed
    for (int j = 0; j < ch_total; j++)
    {
        char buffer[32];
        int counter = 0;
        radio->setChannel(CHs[j]);

        for (int i = 0; i < 100; i++)
        {
            int status = radio->write(buffer, 32); // send 32 bytes of data. It does not matter what it is
            if (status)
                counter++;

            delay(1); // try again in 1 millisecond
        }
        Serial.print("Channel:");
        Serial.print(CHs[j]);
        Serial.print("\t");
        Serial.print("Signal strength: ");
        Serial.println(counter);
    }
    Serial.println(" ");
    _delay_ms(500);
}