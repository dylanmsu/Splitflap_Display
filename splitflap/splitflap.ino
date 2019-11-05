#include <EEPROM.h>

int updateDelay = 80; //delay between each flap
byte Bit = false; //used by "Update" function

//                 H E L  L  O  [] W  O  R  L  D
//int message[] = {8,5,12,12,10,30,53,45,48,42,34};

String message = "abcdefghijklmnopqrstuvwxyz";

void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  Zero(0);
}

void loop() {
  int text[sizeof(message)/sizeof(message[0])];
  stringToIdexArray(message,text,false);
  Zero(0);
  
  for (int i=0;i< 26 ;i++){
    jumpTo(0,text[i]);
    delay(1000);
  }
}

void stringToIdexArray(String input, int *output, boolean red){

  int sizearr = (sizeof(input)/sizeof(input[0]));
  for (int i=0; i<sizearr; i++){
    switch(input[i]){
      case 'a':   output[i] = 1;    break;
      case 'b':   output[i] = 2;    break;
      case 'c':   output[i] = 3;    break;
      case 'd':   output[i] = 4;    break;
      case 'e':   output[i] = 5;    break;
      case 'f':   output[i] = 6;    break;
      case 'g':   output[i] = 7;    break;
      case 'h':   output[i] = 8;    break;
      case 'i':   output[i] = 9;    break;
      case 'j':   output[i] = 10;    break;
      case 'k':   output[i] = 11;    break;
      case 'l':   output[i] = 12;    break;
      case 'm':   output[i] = 13;    break;
      case 'n':   output[i] = 14;    break;
      case 'o':   output[i] = 15;    break;
      case 'p':   output[i] = 16;    break;
      case 'q':   output[i] = 17;    break;
      case 'r':   output[i] = 18;    break;
      case 's':   output[i] = 19;    break;
      case 't':   output[i] = 20;    break;
      case 'u':   output[i] = 21;    break;
      case 'v':   output[i] = 22;    break;
      case 'w':   output[i] = 23;    break;
      case 'x':   output[i] = 24;    break;
      case 'y':   output[i] = 25;    break;
      case 'z':   output[i] = 26;    break;
      case '—':   output[i] = 27;    break;
      case '-':   output[i] = 27;    break;
      case '/':   output[i] = 29;    break;
      case ' ':   output[i] = 30;    break;
      default:    output[i] = 57;    break; //unknown character gets replaced with an "Ö"
    } 
    //if (red){
    //  output[i] += 30;
    //}
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
    
    EEPROM.write(Display, (num+prev)%60);
  }else{
    //error code: reached segment limit
  }
}
