/*
  Teensy 3.6 - Poi Project
         - Receiver Code

  by Felix Geisler
  16.03.2022

*This sketch outputs images to persistence of vision led strips
*It uses FastLed to drive APA102 leds, sending colour values from
*arrays held in flash memory (designated by 'const'). You need to
*set the number of slices you have made your image into, 
*e.g. bmp image of 61 pixels high by 150 wide
* would give 60 num_leds and 
* 150 slices (number of slices you have made your image into) 
*/

#include <Arduino.h>
#include <SPI.h>
// #include <nRF24L01.h>
#include <RF24.h>
#include "FastLED.h"
#include <teensy_3.6_config.h>
#include "..\..\Teensy_4.0\src\visual_functions.h"
#include "..\..\Teensy_4.0\src\visual_functions.cpp"

//#include "send_voltage.h"

#define PIC_IS_SHOWN 25

const byte pipe_address[6] = "00001";
RF24 radio(RF24_CE, RF24_CSN);

// LED
CRGB leds[NUM_LEDS];

// Message Variables
int fillupDone = 0;
int filldownDone = 0;
bool firstStart = true;
unsigned int old_time = 0;
float voltage = 0;
int TeensyData = -1;
message msg;

void setup() 
{
  Serial.begin(115200);                     //Serial
  printf("Initializing...");    
  SPI.setSCK(RF24_SCK);                     //SPI
  SPI.begin();
  delay(200);                               //LED
  FastLED.addLeds<APA102,(uint8_t) DATA_PIN,(uint8_t) CLOCK_PIN, (EOrder) COLOR_ORDER, (uint32_t) DATA_RATE_MHZ(10)>(leds, (int) NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  radio.begin();                            //radio
  radio.openReadingPipe(0, pipe_address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(CHANNEL);
  radio.enableAckPayload(); // Send AckPayload to Master/Arduino
  radio.writeAckPayload(0, &TeensyData, sizeof(TeensyData));
  radio.startListening();
}




// ##########################################################################################
//                                     MAIN LOOP
// ##########################################################################################
void loop()
{
  // Testlight when first start
  if (firstStart)
  {
    StartDemo(DEFAULT_BRIGHTNESS, leds);
    firstStart = false;
  }

  // Data Receive
  if (radio.available())
  {
    byte data[5] = {0};
    radio.read(&data, sizeof(data));
    int status_register = radio.flush_rx();
    msg.mode = data[0];
    msg.picture_hue = data[1];
    msg.saturation = data[2];
    msg.value_brightness = data[3];
    msg.velocity = data[4];
    printf("%i\n%i\n%i\n%i\n%i\n", data[0], data[1], data[2], data[3], data[4]);
    printf("Current Millis: %i\n", millis());
    printf("Current value of status register flush_rx = %d", status_register);
    radio.writeAckPayload(0, &TeensyData, sizeof(TeensyData));
  }

  // Show HSV COLOR 
  if(msg.mode == VIDEO_LIGHT_MODE)
  {
    show_color(msg, leds);
  }

  // Show pictures
  if(msg.mode == PICTURE_MODE)
  {
    display(msg.picture_hue, msg.value_brightness, leds);
  }
  
  //keep alive message
  if(millis() > old_time + 3000)
  {
    voltage = (analogRead(A0)/1024.0) * 3.3 * 2; //3.3V Ref Voltage + 1/2 Voltage Divider
    TeensyData = analogRead(A0);
    printf("\nTeensy 3.6 alive - time: %d\n", millis());
    printf("CHANNEL: %d - VOLTAGE: %f\n\n", CHANNEL, voltage);
    old_time = millis();
  }
}
