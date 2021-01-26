#include <Splitflap.h>

int sensPins[32] = {                               // set all zero because we dont use the sensors
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};              

int serialPins[6] = {44,38,42,36,40,34};                // DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch 
Splitflap splitflaps(sensPins, 200, serialPins);  // make an object of the class (sensor pins, flapdelay, serial pins)

void setup() {
  splitflaps.enableAll(); // enable all the h-bridges
}

void loop() 
{
  for (int i=0; i<32; i++)
  {
    splitflaps.flipSegment(i);
    delay(200);
  }
}
