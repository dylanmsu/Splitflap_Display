#include <Splitflap.h>

int sensPins[8] = {10, 11, 12, 3, A0, A1, A2, A3};   //hall-sensor pins from the splitflaps (order matters)
int serialPins[6] = {38,44,34,40,36,42};              //Data, DataEnable, Latch, LatchEnable, Clock, ClockEnable

Splitflap splitflaps(8, sensPins, 75, serialPins);  //make an object from the class (numSegments, 

void setup() {
}

void loop() {
  splitflaps.enableAll();
  for (int i=0; i<32; i++){
    splitflaps.flipSegment(i);
    delay(100);
  }
  splitflaps.disableAll();
  delay(5000);
}
