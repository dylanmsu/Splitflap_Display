#include "Splitflap.h"

int sensPins[8] = {A0, A1, A2, A3, A4, A5, 2, 3};   //hall-sensor pins from the splitflaps (order matters)
int serialPins[6] = {12,10,8,7,13,11};              //Data, DataEnable, Latch, LatchEnable, Clock, ClockEnable

Splitflap splitflaps(9, sensPins, 65, serialPins);  //make an object from the class (numSegments, 

String incoming = "";
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
      splitflaps.Send(incoming, 0, 0);
      splitflaps.disableAll();
    }
  }
}
