#ifndef SEND_VOLTAGE_INCLUDED
#define SEND_VOLTAGE_INCLUDED

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "FastLED.h"
#include "send_voltage.h"
#include <ADC.h>
#include <t3_vdd.h>
ADC adc;

void send_voltage(byte ch, uint64_t pipe_address, RF24* radio){
  uint16_t mv = t3_vdd::measure_vdd_mV(adc);
  Serial.print("vdd: ");
  Serial.print(mv);
  Serial.println(" mV");

  delay(500);

  radio->stopListening();
  radio->openWritingPipe(pipe_address);
  radio->setChannel(ch);
  int status = radio->write(&mv,sizeof(mv));
  Serial.print("Status: ");
  Serial.println(status);
}

#endif