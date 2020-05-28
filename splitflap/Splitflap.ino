#include "Splitflap.h"

int sensPins[8] = {10, 11, 12, 3, A0, A1, A2, A3};  //hall-sensor pins from the splitflaps (order matters)
int serialPins[6] = {8,9,6,7,4,5};                  //Data, DataEnable, Latch, LatchEnable, Clock, ClockEnable

Splitflap splitflaps(16, sensPins, 65, serialPins);  //make an object from the class

String incoming = "";

void setup() {  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String temp = incoming;
    incoming = Serial.readString();
    incoming.remove(incoming.length()-1);//remove enter
    
    if (incoming.length() > 8){
      Serial.println("err: The string can't contain more than 24 charakters.");
    } else if (incoming == temp && incoming != "") {
      Serial.println("err: That text is already displaying.");
    } else {
      Serial.println(incoming);
      splitflaps.enableAll();
      splitflaps.WriteText(incoming);
      splitflaps.disableAll();
    }
  }
}
