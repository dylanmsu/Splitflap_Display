#include "Splitflap.h"

int sensPins[8] = {50, 51, 48, 49, 46, 47, 44, 45};   //hall-sensor pins from the splitflaps (order matters)               
int serialPins[6] = {A4,A5,A7,A6,A9,A8};              //Data, DataEnable, Latch, LatchEnable, Clock, ClockEnable

Splitflap splitflaps(8, sensPins, 90, serialPins);  //make an object from the class

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
    } else if (incoming == temp) {
      Serial.println("err: That text is already displaying.");
    } else {
      Serial.println(incoming);
      splitflaps.enableAll();
      splitflaps.WriteText(incoming);
      splitflaps.disableAll();
    }
  }
splitflaps.enableAll();

for (int i=0; i<16; i++){
  splitflaps.writeSegment(i,1);
  delay(100);
}


for (int i=0; i<16; i++){
  splitflaps.writeSegment(i,0);
  delay(100);
}
 
}
