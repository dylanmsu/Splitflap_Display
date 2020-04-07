#include <Splitflap.h>

int sensPins[8] = {A0, A1, A2, A3, A4, A5, 2, 3};   //hall-sensor pins from the splitflaps (order matters)
int serialPins[6] = {12,10,8,7,13,11};              //Data, DataEnable, Latch, LatchEnable, Clock, ClockEnable

Splitflap splitflaps(8, sensPins, 75, serialPins);  //make an object from the class (numSegments, 

void setup() {
  splitflaps.enableAll();
  splitflaps.WriteText("whiteRED");
}

void loop() {
  //nothing to loop
}
