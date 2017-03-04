#include <SerialCommand.h>   /* http://github.com/p-v-o-s/Arduino-SerialCommand */

#if defined(CORE_TEENSY)
#include <ADC.h> /* https://github.com/pedvide/ADC */
#else
#include <elapsedMillis.h>
#endif

#include "c12880.h"

#define SPEC_TRG         A0
#define SPEC_ST          A1
#define SPEC_CLK         10
#define SPEC_VIDEO       A3

uint16_t data[C128880_NUM_CHANNELS];
C128880_Class spec(SPEC_TRG,SPEC_ST,SPEC_CLK,SPEC_VIDEO);

SerialCommand sCmd(Serial);// the SerialCommand parser object

void setup(){
  Serial.begin(115200); // Baud Rate set to 115200
  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("SPEC.INTEG", SPEC_INTEG_sCmd_config_handler); //configures integration time
  sCmd.addCommand("SPEC.READ?", SPEC_READ_sCmd_query_handler);   //reads out the whole spectrum
  sCmd.addCommand("SPEC.TIMING?", SPEC_TIMING_sCmd_query_handler);   //reads out the whole spectrum
  spec.begin();
}

void loop(){
  int num_bytes = sCmd.readSerial();      // fill the buffer
  if (num_bytes > 0){
    sCmd.processCommand();  // process the command
  }
  delay(10);
}

void SPEC_INTEG_sCmd_config_handler(SerialCommand this_sCmd){
  char *arg = this_sCmd.next();
  float integ_time;
  if (arg == NULL){
    this_sCmd.print(F("### Error: SPEC.INTEG requires 1 argument 'time' (s)\n"));
  }
  else{
    integ_time = atof(arg);
    spec.set_integration_time(integ_time);
  }
}

void SPEC_READ_sCmd_query_handler(SerialCommand this_sCmd){
  spec.read_into(data);
  for (int i = 0; i < C128880_NUM_CHANNELS - 1; i++){
    this_sCmd.print(data[i]);
    this_sCmd.print(',');
  }
  //last value gets special formatting
  this_sCmd.print(data[C128880_NUM_CHANNELS - 1]);
  this_sCmd.print("\n");
}

void SPEC_TIMING_sCmd_query_handler(SerialCommand this_sCmd){
  for(int i=0; i < 9; i++){
    this_sCmd.print(spec.get_timing(i));
    this_sCmd.print(",");
  }
  this_sCmd.print(spec.get_timing(9));
  this_sCmd.print("\n");
}
