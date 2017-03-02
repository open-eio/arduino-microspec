/*
  c12880.cpp - Library for interacting with the Hamamatsu C128880
                          microspectrometer
  Created by Craig Wm. Versek, 2017-03-01
  
  Much of the initial implmentation was based on this project:
  https://github.com/groupgets/c12880ma/blob/master/arduino_c12880ma_example/arduino_c12880ma_example.ino
 */
#include <Arduino.h>
#include <ADC.h> /* https://github.com/pedvide/ADC */
#include "c12880.h"
 
C128880_Class::C128880_Class(const int TRG_pin,
                             const int ST_pin,
                             const int CLK_pin,
                             const int VIDEO_pin
                             ){
  _TRG_pin = TRG_pin;
  _ST_pin  = ST_pin;
  _CLK_pin = CLK_pin;
  _VIDEO_pin = VIDEO_pin;
  _adc = new ADC(); // adc object
}

void C128880_Class::begin() {
  ///// ADC0 ////--------------------------------------------------------------
  //adc->setReference(ADC_REF_INTERNAL, ADC_0); For Teensy 3.x ADC_REF_INTERNAL is 1.2V, default is 3.3V
  _adc->setAveraging(1); // set number of averages
  _adc->setResolution(16); // set bits of resolution
  // it can be VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED_16BITS, HIGH_SPEED or VERY_HIGH_SPEED
  // see the documentation for more information
  _adc->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED_16BITS); // change the conversion speed
  // it can be VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED or VERY_HIGH_SPEED
  _adc->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED); // change the sampling speed

  //Set desired pins to OUTPUT
  pinMode(_CLK_pin, OUTPUT);
  pinMode(_ST_pin, OUTPUT);
  
  digitalWrite(_CLK_pin, HIGH); // Start with CLK High
  digitalWrite(_ST_pin, LOW);   // Start with ST Low
}

void C128880_Class::read_into(uint16_t *buffer) {
  int delayTime = 1; // delay time
  // Start clock cycle and set start pulse to signal start
  digitalWrite(_CLK_pin, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(_CLK_pin, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(_CLK_pin, LOW);
  digitalWrite(_ST_pin, HIGH);
  delayMicroseconds(delayTime);
  
  //Sample for a period of time
  for(int i = 0; i < 15; i++){

      digitalWrite(_CLK_pin, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(_CLK_pin, LOW);
      delayMicroseconds(delayTime);
 
  }
  
  //Set _ST_pin to low
  digitalWrite(_ST_pin, LOW);

  //Sample for a period of time
  for(int i = 0; i < 85; i++){

      digitalWrite(_CLK_pin, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(_CLK_pin, LOW);
      delayMicroseconds(delayTime); 
      
  }

  //One more clock pulse before the actual read
  digitalWrite(_CLK_pin, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(_CLK_pin, LOW);
  delayMicroseconds(delayTime);

  //Read from SPEC_VIDEO
  for(int i = 0; i < C128880_NUM_CHANNELS; i++){

      buffer[i] = _adc->analogRead(_VIDEO_pin);
      
      digitalWrite(_CLK_pin, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(_CLK_pin, LOW);
      delayMicroseconds(delayTime);
        
  }

  //Set _ST_pin to high
  digitalWrite(_ST_pin, HIGH);

  //Sample for a small amount of time
  for(int i = 0; i < 7; i++){
    
      digitalWrite(_CLK_pin, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(_CLK_pin, LOW);
      delayMicroseconds(delayTime);
    
  }

  digitalWrite(_CLK_pin, HIGH);
  delayMicroseconds(delayTime);
}
