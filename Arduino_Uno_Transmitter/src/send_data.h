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
    uint8_t size_1 = (uint8_t)sizeof(*content);
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