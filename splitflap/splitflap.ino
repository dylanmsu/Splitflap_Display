#include <EEPROM.h>

//int numDisplays = 3;
int updateDelay = 100;
byte Bit = false;

void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT_PULLUP);
  EEPROM.write(0,0);
}

void loop() {
  //Update(0, 100);
  increase(0,5);
  delay(1000);
}

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
}

void Update(byte Display, int flapDelay){
  digitalWrite(12, Bit);
  digitalWrite(13, Bit);
  delay(flapDelay);
  Bit = !Bit;
}

void Zero(byte Display){
  while(digitalRead(A0)){
    Update(Display, updateDelay);
  }
  EEPROM.write(0,0);
}

void increase(byte Display, int num){
  int prev = EEPROM.read(Display);
  if (num+prev < 54){
    
    for (int i=0; i<num; i++){
      Update(Display, updateDelay);
    }
    
    EEPROM.write(Display, num+prev%53);
  }else{
    //error code: reached segment limit
  }
}

/*void drawChar(int displ, int state){
  int flapDelay = 100;
  
  if (state < prevState){
    while(hallSensor == 0){
      Update();
    }
    prevState = 0;
  }
  
  for (int i = prevState; i<state; i++){
    Update();
  }
}*/
