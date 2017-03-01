#include "c12880.h"

#define SPEC_TRG         A0
#define SPEC_ST          A1
#define SPEC_CLK         A2
#define SPEC_VIDEO       A3

uint16_t data[C128880_NUM_CHANNELS];
C128880_Class spec(SPEC_TRG,SPEC_ST,SPEC_CLK,SPEC_VIDEO);

void setup(){
  Serial.begin(115200); // Baud Rate set to 115200
  spec.begin();
}

void loop(){
  spec.read_into(data);
  for (int i = 0; i < C128880_NUM_CHANNELS; i++){
    Serial.print(data[i]);
    Serial.print(',');
  }
  Serial.print("\n");
  delay(10);
}
