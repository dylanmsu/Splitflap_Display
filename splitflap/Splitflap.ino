#include "Splitflap.h"
String incoming = "";
//hall-sensor pins from the splitflaps (order matters)
int sensPins[32] = {
  53, 52, 51, 50, 49, 48, 47, 46,
  43, 42, 41, 40, 39, 38, 37, 36,
  35, 34, 33, 32, 31, 30, 29, 28,
  A8, A9, A10,A11,A12,A13,A14,A15
};    
                
int serialPins[6] = {A0,A1,A2,A3,A4,A5};              //DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch 
Splitflap splitflaps(32, sensPins, 100, serialPins);    //make an object from the class

void setup() {  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    delay(10);
    incoming = Serial.readString();
    incoming.remove(incoming.length()-1);//remove enter
    
    if (incoming.length() > 8){
      Serial.println("err: the string can't contain more than 24 charakters");
    } else {
      Serial.println(incoming);
      splitflaps.enableAll();
      splitflaps.WriteText(incoming);
      splitflaps.disableAll();
    }
  }
}
