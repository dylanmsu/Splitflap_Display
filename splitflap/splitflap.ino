#include <EEPROM.h>

int updateDelay = 64; //delay between each flap
byte Bit = false; //used by "Update" function

String message = "hello world"; //read from serial
String temp = "";

void setup() {
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  Zero(0);
  Serial.begin(115200);
  //Serial.write(int(sizeof(message[0])));
}

void loop() {
  Zero(0);

  //Serial.write(int(sizeof(message)/sizeof(message[0])));
  /*while (Serial.available()) {
    delay(20);
    char c = Serial.read();
    temp += c;
  }*/

  for (int i=0;i<message.length(); i++){
    jumpTo(0,lookup(message[i],false));
    delay(500);
  }
  delay(1000);
}

int lookup(char input, boolean red){
  int output = 0;
  switch(input){
    case 'a':   output = 1;    break;
    case 'b':   output = 2;    break;
    case 'c':   output = 3;    break;
    case 'd':   output = 4;    break;
    case 'e':   output = 5;    break;
    case 'f':   output = 6;    break;
    case 'g':   output = 7;    break;
    case 'h':   output = 8;    break;
    case 'i':   output = 9;    break;
    case 'j':   output = 10;    break;
    case 'k':   output = 11;    break;
    case 'l':   output = 12;    break;
    case 'm':   output = 13;    break;
    case 'n':   output = 14;    break;
    case 'o':   output = 15;    break;
    case 'p':   output = 16;    break;
    case 'q':   output = 17;    break;
    case 'r':   output = 18;    break;
    case 's':   output = 19;    break;
    case 't':   output = 20;    break;
    case 'u':   output = 21;    break;
    case 'v':   output = 22;    break;
    case 'w':   output = 23;    break;
    case 'x':   output = 24;    break;
    case 'y':   output = 25;    break;
    case 'z':   output = 26;    break;
    case '—':   output = 27;    break;
    case '-':   output = 27;    break;
    case '/':   output = 29;    break;
    case ' ':   output = 30;    break;
    default:    output = 57;    break; //unknown character gets replaced with an "Ö"
  } 
  if (red){
    output += 30;
  }
  return output;
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
  //switch(Display){
  //  case 0:
      digitalWrite(12, Bit);
      digitalWrite(13, Bit);
  //  break;
  //  case 1:
      digitalWrite(11, Bit);
  //  break;
  //}
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
    
    EEPROM.write(Display, (num+prev)%60);
  }else{
    //error code: reached segment limit
  }
}
