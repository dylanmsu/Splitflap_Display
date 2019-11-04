#include <EEPROM.h>

int updateDelay = 70; //delay between each flap
byte Bit = false; //used by "Update" function

//               H E L  L  O  [] W  O  R  L  D
int message[] = {8,5,12,12,10,30,53,45,48,42,34};

void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  EEPROM.write(0,0);
}

void loop() {
  int *text;
  stringToIdexArray("HELLO WORLD",text,false);
  
  for (int i=0;i<sizeof(text);i++){
    jumpTo(0,text[i],false);
    delay(1000);
  }
}

void stringToIdexArray(String input, int *output, boolean red){
  //TODO
  //convert a string to a list of segment numbers
  for (int i=0;i<sizeof(input);i++){
    switch(input[i]){
      case 'A':   output[i] = 1;    break;
      case 'B':   output[i] = 2;    break;
      case 'C':   output[i] = 3;    break;
      case 'D':   output[i] = 4;    break;
      case 'E':   output[i] = 5;    break;
      case 'F':   output[i] = 6;    break;
      case 'G':   output[i] = 7;    break;
      case 'H':   output[i] = 8;    break;
      case 'I':   output[i] = 9;    break;
      case 'J':   output[i] = 10;    break;
      case 'K':   output[i] = 11;    break;
      case 'L':   output[i] = 12;    break;
      case 'M':   output[i] = 13;    break;
      case 'N':   output[i] = 14;    break;
      case 'O':   output[i] = 15;    break;
      case 'P':   output[i] = 16;    break;
      case 'Q':   output[i] = 17;    break;
      case 'R':   output[i] = 18;    break;
      case 'S':   output[i] = 19;    break;
      case 'T':   output[i] = 20;    break;
      case 'U':   output[i] = 21;    break;
      case 'V':   output[i] = 22;    break;
      case 'W':   output[i] = 23;    break;
      case 'X':   output[i] = 24;    break;
      case 'Y':   output[i] = 25;    break;
      case 'Z':   output[i] = 26;    break;
      case '—':   output[i] = 23;    break;
      case '-':   output[i] = 24;    break;
      case '/':   output[i] = 25;    break;
      case ' ':   output[i] = 26;    break;
      default:    output[i] = 57;    break; //unknown character gets replaced with an "Ö"
    } 
    if (red){
      output[i] += 30;
    }
  }
}

//jump to a character on the display
void jumpTo(byte Display, int num, boolean red){
  int prev = EEPROM.read(Display);
  
  if (num < prev){
    Zero(Display);
    if (red){
      increase(Display,num+30);
    }else{
      increase(Display,num);
    }
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
  digitalWrite(11, Bit);
  digitalWrite(10, Bit);
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
