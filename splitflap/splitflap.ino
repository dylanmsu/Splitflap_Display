#include <EEPROM.h>

int updateDelay = 80; //delay between each flap
byte Bit = false; //used by "Update" function

void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  EEPROM.write(0,0);
}

void loop() {
  Zero(0); 
  jumpTo(0,8);
  delay(1000);
  Zero(0);
  jumpTo(0,5);
  delay(1000);
  Zero(0);
  jumpTo(0,12);
  delay(1000);
  Zero(0);
  jumpTo(0,12);
  delay(1000);
  Zero(0);
  jumpTo(0,15);
  delay(1000);
}

//jump to a character on the display
void jumpTo(byte Display, int num){
  int prev = EEPROM.read(Display);
  if (num < prev){
    Zero(Display);
    for (int i=0; i<num;i++){
      Update(Display, updateDelay);
    }
  }else if (num > prev){
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

//flaps a cirtain amount of times
void increase(byte Display, int num){
  int prev = EEPROM.read(Display);
  if (num+prev < 54){
    
    for (int i=0; i<num; i++){
      Update(Display, updateDelay);
    }
    
    EEPROM.write(Display, (num+prev)%54);
  }else{
    //error code: reached segment limit
  }
}
