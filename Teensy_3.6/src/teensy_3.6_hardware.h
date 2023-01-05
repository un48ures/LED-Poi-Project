//NRF240 HW Pins - Radio settings
#define RF24_MOSI 11
#define RF24_MISO 12
#define RF24_SCK 14
#define CHANNEL 60 //CHANNEL: 20 30 40 50 60 70 -> POI (1 2 3 4 5 6) 
//Poi 1 - 4 -> Teensy 4.0 -> Channel 20 - 50
//Poi 5 & 6 -> Teensy 3.6 -> Channel 60 & 70

RF24 radio(17, 18); // CE, CSN

//FastLED HW Pins
#define NUM_LEDS   61 //number of leds in strip length on one side
#define DATA_PIN   0  //7 = second hardware spi data
#define CLOCK_PIN  32 //14 = second hardware spi clock
#define COLOR_ORDER BGR