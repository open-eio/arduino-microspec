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
  _adc = new ADC();        // adc object
  _clock_delay_micros = 1; // half of a clock period
  set_integration_time(0.001);  // integration time default to 1ms
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
  
  digitalWrite(_CLK_pin, LOW); // Start with CLK High
  digitalWrite(_ST_pin, LOW);  // Start with ST Low
}

void C128880_Class::set_integration_time(float seconds) {
  _integ_time = max(seconds, 48*2*1e-6);
  _integ_clock_cycles = (int)(1000000*_clock_delay_micros/2)*_integ_time - 48;
  _integ_clock_cycles = max(_integ_clock_cycles,0);
}

void C128880_Class::_pulse_clock(int times){
  for(int i = 0; i < times; i++){
    digitalWrite(_CLK_pin, HIGH);
    delayMicroseconds(_clock_delay_micros);
    digitalWrite(_CLK_pin, LOW);
    delayMicroseconds(_clock_delay_micros);
  }
}

void C128880_Class::read_into(uint16_t *buffer) {
  // Start clock cycle and set start pulse to signal start
  digitalWrite(_CLK_pin, HIGH);
  delayMicroseconds(_clock_delay_micros);
  digitalWrite(_CLK_pin, LOW);
  digitalWrite(_ST_pin, HIGH);
  delayMicroseconds(_clock_delay_micros);
  _pulse_clock(3); //pixel integration starts after three clock pulses
  
  //Integrate pixels for a while
  _pulse_clock(_integ_clock_cycles);
  
  //Set _ST_pin to low
  digitalWrite(_ST_pin, LOW);
 
  //Sample for a period of time
  //integration stops at pulse 
  //pixel output is ready after last pulse
  _pulse_clock(88);

  //Read from SPEC_VIDEO
  for(int i = 0; i < C128880_NUM_CHANNELS; i++){
    buffer[i] = _adc->analogRead(_VIDEO_pin);
    _pulse_clock(1);
  }
}
