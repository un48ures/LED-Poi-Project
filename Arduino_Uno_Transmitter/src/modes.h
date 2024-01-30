#ifndef MODES
#define MODES
#include <RF24.h>

void midi_mode(RF24 *radio, int total_number_receivers);
void video_light_mode(RF24* radio);
void new_remote_control();
void send_data(uint8_t ch, byte *content, uint8_t size, const uint8_t *pipe_address, RF24 *radio);
void print_signal_strength(RF24 *radio, const byte *CHs, int8_t ch_total);
#endif