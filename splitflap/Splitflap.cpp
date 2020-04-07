#include "Arduino.h"
#include "Splitflap.h"


Splitflap::Splitflap(int numSegments, int *serialPins){

  //serialPins = {D,DE,La,LaE,Clk,ClkE};
  
  updateDelay = 75; //delay between each flap in milliseconds
  num = numSegments;

  int sensPins[] = {A0, A1, A2, A3, A4, A5, 2, 3};
  for (int i=0; i<num; i++){
    APins[i] = sensPins[i];
    pinMode(APins[i], INPUT_PULLUP);
  }
  
  latch_enable = serialPins[3];
  clock_enable = serialPins[5];
  data_enable = serialPins[1];
  
  latchH = serialPins[2];
  clockH = serialPins[4];
  dataH = serialPins[0];

  for (int i=0; i<6; i++){
    pinMode(serialPins[i], OUTPUT);
  }
  
  bitsToSendA = 0x00;
  bitsToSendB = 0xff;
};


Splitflap::Splitflap(int numSegments, int *sensPins, int *serialPins){
  updateDelay = 75; //delay between each flap in milliseconds
  num = numSegments;

  for (int i=0; i<num; i++){
    APins[i] = sensPins[i];
    pinMode(APins[i], INPUT_PULLUP);
  }
  
  latch_enable = serialPins[3];
  clock_enable = serialPins[5];
  data_enable = serialPins[1];
  
  latchH = serialPins[2];
  clockH = serialPins[4];
  dataH = serialPins[0];

  for (int i=0; i<6; i++){
    pinMode(serialPins[i], OUTPUT);
  }
  
  bitsToSendA = 0x00;
  bitsToSendB = 0xff;
};

Splitflap::Splitflap(int numSegments, int updateDelayMs, int *serialPins){
  updateDelay = updateDelayMs; //delay between each flap in milliseconds
  num = numSegments;

  int sensPins[] = {A0, A1, A2, A3, A4, A5, 2, 3};
  for (int i=0; i<num; i++){
    APins[i] = sensPins[i];
    pinMode(APins[i], INPUT_PULLUP);
  }
  
  latch_enable = serialPins[3];
  clock_enable = serialPins[5];
  data_enable = serialPins[1];
  
  latchH = serialPins[2];
  clockH = serialPins[4];
  dataH = serialPins[0];

  for (int i=0; i<6; i++){
    pinMode(serialPins[i], OUTPUT);
  }
  
  bitsToSendA = 0x00;
  bitsToSendB = 0xff;
};

Splitflap::Splitflap(int numSegments, int *sensPins,  int updateDelayMs, int *serialPins){
  updateDelay = updateDelayMs; //delay between each flap in milliseconds
  num = numSegments;

  for (int i=0; i<num; i++){
    APins[i] = sensPins[i];
    pinMode(APins[i], INPUT_PULLUP);
  }
  
  latch_enable = serialPins[3];
  clock_enable = serialPins[5];
  data_enable = serialPins[1];
  
  latchH = serialPins[2];
  clockH = serialPins[4];
  dataH = serialPins[0];

  for (int i=0; i<6; i++){
    pinMode(serialPins[i], OUTPUT);
  }
  
  bitsToSendA = 0x00;
  bitsToSendB = 0xff;
};

//continues flapping until all segments are on the 0'th position
void Splitflap::Zero(){
  setAll(state, 1, num);
  while (!isAllZero(state)){
    for (int i = 0; i<num; i++){
      delay(updateDelay/num);
      int hall = digitalRead(APins[i]);
      state[i] = hall;
      if (state[i]){
        flipSegment(i);
      }else{
      }
    }
  }
};
    
void Splitflap::WriteText(String text){
  Zero(); // set all displays to zero

  //check if letter is uppercase and change the color of all uppercase letters to red
  String temp = text;
  text.toLowerCase();
  for (int i=0; i<text.length(); i++){
    if (temp[i] != text[i]){
      indices[i] = lookup(text[i],true);
    }else{
      indices[i] = lookup(text[i],false);
    }
  }
  writeIndices(indices);
};
    
int Splitflap::lookup(char input, boolean red){
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
    case '-':   output = 28;    break;
    case '/':   output = 29;    break;
    case ' ':   output = 30;    break;
    default:    output = 57;    break; //unknown character gets replaced with an "Ö"
  } 
  if (red){
    output += 30;
  }
  return output;
};

void Splitflap::setAll(int *arr, int to, int len){
  for (int i=0; i<num; i++){
    arr[i] = to;
  }
};

bool Splitflap::isAllZero(int *arr){
  bool zero = 0;
  for (int i=0; i<num; i++){
    zero += arr[i];
  }
  return !zero;
};

void Splitflap::writeIndices(int *indices){
  while (!isAllZero(indices)){
    for (int i=0; i<num; i++){
      if (indices[i]){
        indices[i] -= 1;
        flipSegment(i);
      }
      delay(updateDelay/num);
    }
  }
};

void Splitflap::flipSegment(int segment){
  writeSegment(segment, !Bit[segment]);
  Bit[segment] = !Bit[segment];
};

void Splitflap::writeSegment(int whichPin, int whichState) {
  digitalWrite(latchH, LOW);

  bitWrite(bitsToSendA, whichPin, whichState);
  shiftOut(dataH, clockH, MSBFIRST, bitsToSendA);

  digitalWrite(latchH, HIGH);
};

void Splitflap::enableSegment(int which, int whichState) {
  digitalWrite(latch_enable, LOW);

  bitWrite(bitsToSendB, which, whichState);
  shiftOut(data_enable, clock_enable, MSBFIRST, bitsToSendB);  

  digitalWrite(latch_enable, HIGH);
};

void Splitflap::enableAll() {
  digitalWrite(latch_enable, LOW);

  shiftOut(data_enable, clock_enable, MSBFIRST, 0xFF);  

  digitalWrite(latch_enable, HIGH);
};

void Splitflap::disableAll() {
  digitalWrite(latch_enable, LOW);

  shiftOut(data_enable, clock_enable, MSBFIRST, 0x00);  

  digitalWrite(latch_enable, HIGH);
};
