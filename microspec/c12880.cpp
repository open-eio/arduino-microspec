/*
  c12880.cpp - Library for interacting with the Hamamatsu C128880
                          microspectrometer
  Created by Craig Wm. Versek, 2017-03-01
 */
#include <Arduino.h>
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
}

void C128880_Class::begin() {
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

      buffer[i] = analogRead(_VIDEO_pin);
      
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
