#include "Splitflap.h"

String incoming = "";
Splitflap splitflaps(8);

void setup() {
  const int latch1 = 7;
  const int clock1 = 11;
  const int data1 = 10;
  
  const int latch2 = 8;
  const int clock2 = 13;
  const int data2 = 12;
  
  pinMode(latch1, OUTPUT);
  pinMode(data1, OUTPUT);  
  pinMode(clock1, OUTPUT);
  pinMode(latch2, OUTPUT);
  pinMode(data2, OUTPUT);  
  pinMode(clock2, OUTPUT);
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    splitflaps.enableAll();
    incoming = Serial.readString();
    incoming.remove(incoming.length()-1);//remove enter
    
    if (incoming.length() > 8){
      String a = "err: the string can't contain more than ";
      String b = " charakters.";
      Serial.println(a + 8 + b);
    } else {
      Serial.println(incoming);
      splitflaps.WriteText(incoming);
    }
    delay(300);
    splitflaps.disableAll();
  }
}
