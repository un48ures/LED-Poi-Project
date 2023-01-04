#include <RF24.h>

void receive_voltage(RF24 *radio, uint64_t pipe_address)
{
    radio->startListening();
    radio->openReadingPipe(0, pipe_address);
    radio->setChannel(60);

    if (radio->available())
    {
        uint16_t volt;
        radio->read(&volt, sizeof(volt));
        Serial.print("Voltage: ");
        Serial.print(volt);
        Serial.println(" mV");
    }
}