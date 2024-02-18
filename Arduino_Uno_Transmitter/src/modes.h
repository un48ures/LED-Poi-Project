#ifndef MODES
#define MODES
#include <RF24.h>

void midi_mode(RF24 *radio, int total_number_receivers, receiver* teensy);
void video_light_mode(RF24* radio, receiver* teensy);
void new_remote_control();
int send_data(receiver* teensy, byte *data, uint8_t size, const uint8_t *pipe_address, RF24 *radio, int retries, int retry_delay);
void print_signal_strength(RF24 *radio, receiver* teensy, int8_t total);
#endif