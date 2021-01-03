#include <Splitflap.h>

int sensPins[8] = {                               // set all zero because we dont use the sensors
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};              

int serialPins[6] = {2,3,4,5,6,7};                // DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch 
Splitflap splitflaps(sensPins, 200, serialPins);  // make an object of the class (sensor pins, flapdelay, serial pins)

void setup() {
  splitflaps.enableAll(); // enable all the h-bridges
}

void loop() 
{
  for (int i=0; i<8; i++)
  {
    splitflaps.flipSegment(i);
    delay(500);
  }
  delay(1000);
}