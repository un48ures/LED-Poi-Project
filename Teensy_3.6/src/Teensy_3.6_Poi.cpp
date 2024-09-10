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
// #include "FastLED.h"
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
    byte data[6] = {0};
    radio.read(&data, sizeof(data));
    int status_register = radio.flush_rx();
    msg.mode = data[0];
    msg.picture = data[1];
    msg.hue = data[2];
    msg.saturation = data[3];
    msg.value_brightness = data[4];
    msg.velocity = data[5];
    printf("%i\n%i\n%i\n%i\n%i\n", data[0], data[1], data[2], data[3], data[4], data[5]);
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
    display(msg, leds);
  }
  
  //keep alive message
  if(millis() > old_time + 3000)
  {
    voltage = (analogRead(A1)/1024.0) * 3.3 * 2; //3.3V Ref Voltage + 1/2 Voltage Divider
    TeensyData = analogRead(A1);
    printf("\nTeensy 3.6 alive - time: %d\n", millis());
    // printf("Array 1 length : %i and size:%i\n", sizeof(array1)/sizeof(array1[0]), sizeof(array1));
    // printf("Array 2 length : %i and size:%i\n", sizeof(array2)/sizeof(array2[0]), sizeof(array2));
    // printf("Array 3 length : %i and size:%i\n", sizeof(array3)/sizeof(array3[0]), sizeof(array3));
    // printf("Array 4 length : %i and size:%i\n", sizeof(array4)/sizeof(array4[0]), sizeof(array4));
    // printf("Array 5 length : %i and size:%i\n", sizeof(array5)/sizeof(array5[0]), sizeof(array5));
    // printf("Array 6 length : %i and size:%i\n", sizeof(array6)/sizeof(array6[0]), sizeof(array6));
    // printf("Array 7 length : %i and size:%i\n", sizeof(array7)/sizeof(array7[0]), sizeof(array7));
    // printf("Array 8 length : %i and size:%i\n", sizeof(array8)/sizeof(array8[0]), sizeof(array8));
    // printf("Array 9 length : %i and size:%i\n", sizeof(array9)/sizeof(array9[0]), sizeof(array9));
    // printf("Array 10 length : %i and size:%i\n", sizeof(array10)/sizeof(array10[0]), sizeof(array10));
    // printf("Array 11 length : %i and size:%i\n", sizeof(array11)/sizeof(array11[0]), sizeof(array11));
    // printf("Array 12 length : %i and size:%i\n", sizeof(array12)/sizeof(array12[0]), sizeof(array12));
    // printf("Array 13 length : %i and size:%i\n", sizeof(array13)/sizeof(array13[0]), sizeof(array13));
    // printf("Array 14 length : %i and size:%i\n", sizeof(array14)/sizeof(array14[0]), sizeof(array14));
    // printf("Array 15 length : %i and size:%i\n", sizeof(array15)/sizeof(array15[0]), sizeof(array15));
    // printf("Array 16 length : %i and size:%i and size of array16[0]: %i\n", sizeof(array16)/sizeof(array16[0]), sizeof(array16), sizeof(array16[0]));
    printf("CHANNEL: %d - VOLTAGE: %f\n\n", CHANNEL, voltage);
    old_time = millis();
  }
}
