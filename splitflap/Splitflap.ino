#include "Splitflap.h"

int sensPins[8] = {50, 51, 48, 49, 46, 47, 44, 45};   //hall-sensor pins from the splitflaps (order matters)               
int serialPins[6] = {A4,A5,A7,A6,A9,A8};              //Data, DataEnable, Latch, LatchEnable, Clock, ClockEnable

Splitflap splitflaps(8, sensPins, 90, serialPins);  //make an object from the class

String incoming = "";

void setup() {  
  Serial.begin(9600);
}

void loop() {
  
}
