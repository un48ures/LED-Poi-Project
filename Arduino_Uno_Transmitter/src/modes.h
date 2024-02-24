#ifndef MODES
#define MODES
#include <RF24.h>

void midi_mode(RF24 *radio, int total_number_receivers, receiver* teensy);
void video_light_mode_HW(RF24* radio, receiver* teensy);
void pass_on_message(RF24* radio, receiver* teensy, message message_from_pc);
int send_data(receiver* teensy, byte *data, uint8_t size, const uint8_t *pipe_address, RF24 *radio, int retries, int retry_delay);
void signal_strength(RF24 *radio, receiver* teensy, int8_t total);
void send_info(receiver* teensy);
#endif