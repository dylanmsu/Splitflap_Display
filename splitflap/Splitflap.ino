#include "Splitflap.h"

int sensPins[8] = {50, 51, 48, 49, 46, 47, 44, 45};   //hall-sensor pins from the splitflaps (order matters)               
int serialPins[6] = {A0,A1,A2,A3,A4,A5};              //DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch 
Splitflap splitflaps(8, sensPins, 90, serialPins);    //make an object from the class

String incoming = "";

void setup() {  
  Serial.begin(9600);
  splitflaps.enableAll();
}

void loop() {
  for (int j=0; j<2; j++){
    for (int i=0; i<8; i++){
      splitflaps.writeSegment(i, j);
      delay(100);
    }
  }
}
