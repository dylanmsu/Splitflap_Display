#include <EEPROM.h>

int updateDelay = 60; //delay between each flap
byte Bit = false; //used by "Update" function

//                H E  L L  O  []  W  O R  L  D
int message[8] = {8,5,12,12,10,30,53,45,48,42,34};

void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  EEPROM.write(0,0);
}

void loop() {
  Zero(0);
  for (int i=0;i<8;i++){
    jumpTo(0,message[i]);
    delay(1000);
  }
}

//jump to a character on the display
void jumpTo(byte Display, int num){
  int prev = EEPROM.read(Display);
  
  if (num < prev){
    Zero(Display);
    increase(Display,num);
  }
  else if (num > prev){
    for (int i=prev; i<num;i++){
      Update(Display, updateDelay);
    }
  }
  EEPROM.write(Display,num);
}

//flap once
void Update(byte Display, int flapDelay){
  digitalWrite(12, Bit);
  digitalWrite(13, Bit);
  delay(flapDelay);
  Bit = !Bit;
}

//flaps until it reaches the first display-segment
void Zero(byte Display){
  while(digitalRead(A0)){
    Update(Display, updateDelay);
  }
  EEPROM.write(0,0);
}

//flaps a certain amount of times
void increase(byte Display, int num){
  int prev = EEPROM.read(Display);
  if (num+prev < 60){
    
    for (int i=0; i<num; i++){
      Update(Display, updateDelay);
    }
    
    EEPROM.write(Display, (num+prev)%54);
  }else{
    //error code: reached segment limit
  }
}
