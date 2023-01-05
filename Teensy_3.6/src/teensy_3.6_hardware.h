//NRF240 HW Pins - Radio settings
#define RF24_MOSI 11
#define RF24_MISO 12
#define RF24_SCK 14


RF24 radio(17, 18); // CE, CSN

//FastLED HW Pins
#define NUM_LEDS   61 //number of leds in strip length on one side
#define DATA_PIN   0  //7 = second hardware spi data
#define CLOCK_PIN  32 //14 = second hardware spi clock
#define COLOR_ORDER BGR