#include "arduino.h"
#include "Splitflap.h"


Splitflap::Splitflap(int numSegments, int *sensPins,  int updateDelayMs, int *serialPins){
  updateDelay = updateDelayMs; //delay between each flap in milliseconds
  num = numSegments;

  for (int i=0; i<num; i++){
    APins[i] = sensPins[i];
    pinMode(APins[i], INPUT_PULLUP);
  }

  setAll(Bit, 0, num);
  
  latch_enable = serialPins[4];
  clock_enable = serialPins[2];
  data_enable = serialPins[0];
  
  latchH = serialPins[5];
  clockH = serialPins[3];
  dataH = serialPins[1];


  for (int i=0; i<6; i++){
    pinMode(serialPins[i], OUTPUT);
  }
  
  stateSegment = 0x0000;
  stateEnable = 0x0000;
};

//continues flapping until all segments are on the 0'th position
void Splitflap::ResetAll(){
  setAll(state, 1, num);
  while (!isAllZero(state)){
    for (int i = 0; i<num; i++){
      delay(updateDelay/num);
      state[i] = digitalRead(APins[i]);
      if (state[i]){
        flipSegment(i);
      }
    }
  }
  delay(updateDelay);
};
    
void Splitflap::WriteText(String text){
  if (text.length() <= 28){
    enableAll();
    ResetAll(); // set all displays to zero
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
    disableAll();
  } else {
    // error: 
  }
  
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
    case '-':   output = 27;    break;
    case '_':   output = 58;    break;
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
  for (int i=0; i<len; i++){
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
  delay(updateDelay);
};

void Splitflap::flipSegment(int segment){
  writeSegment(segment, !Bit[segment]);
  Bit[segment] = !Bit[segment];
};

void Splitflap::writeSegment(int whichPin, int whichState) {
  digitalWrite(latchH, LOW);

  /*if (whichState){
    stateSegment |= (1ULL << (whichPin));
  }else{
    stateSegment &= -(1ULL << (whichPin));
  } */

  bitWrite(stateSegment,whichPin,whichState);

  uint8_t first =  (stateSegment & 0xFF000000UL) >> 24;
  uint8_t second = (stateSegment & 0x00FF0000UL) >> 16;
  uint8_t third =  (stateSegment & 0x0000FF00UL) >> 8;
  uint8_t forth  = (stateSegment & 0x000000FFUL);
  
  shiftOut(dataH, clockH, MSBFIRST, first);
  shiftOut(dataH, clockH, MSBFIRST, second);
  shiftOut(dataH, clockH, MSBFIRST, third);
  shiftOut(dataH, clockH, MSBFIRST, forth);

  digitalWrite(latchH, HIGH);
};

void Splitflap::writeEnable(int whichPin, int whichState) {
  digitalWrite(latch_enable, LOW);
  
  if (whichState){
    stateEnable |= (1ULL << (whichPin));
  }else{
    stateEnable &= !(1ULL << (whichPin));
  }
  
  uint8_t first =  (stateEnable & 0xFF000000UL) >> 24;
  uint8_t second = (stateEnable & 0x00FF0000UL) >> 16;
  uint8_t third =  (stateEnable & 0x0000FF00UL) >> 8;
  uint8_t forth  = (stateEnable & 0x000000FFUL);
  
  shiftOut(data_enable, clock_enable, MSBFIRST, first);
  shiftOut(data_enable, clock_enable, MSBFIRST, second);
  shiftOut(data_enable, clock_enable, MSBFIRST, third);
  shiftOut(data_enable, clock_enable, MSBFIRST, forth);

  digitalWrite(latch_enable, HIGH);
};

void Splitflap::enableAll() {
  digitalWrite(latch_enable, LOW);

  shiftOut(data_enable, clock_enable, MSBFIRST, 0xFF);  
  shiftOut(data_enable, clock_enable, MSBFIRST, 0xFF);  
  shiftOut(data_enable, clock_enable, MSBFIRST, 0xFF);  
  shiftOut(data_enable, clock_enable, MSBFIRST, 0xFF);  

  digitalWrite(latch_enable, HIGH);
  stateEnable = 0xFFFFFFFF;
};

void Splitflap::disableAll() {
  digitalWrite(latch_enable, LOW);

  shiftOut(data_enable, clock_enable, MSBFIRST, 0x00); 
  shiftOut(data_enable, clock_enable, MSBFIRST, 0x00); 
  shiftOut(data_enable, clock_enable, MSBFIRST, 0x00); 
  shiftOut(data_enable, clock_enable, MSBFIRST, 0x00); 

  digitalWrite(latch_enable, HIGH);
  stateEnable = 0x00000000;
};
