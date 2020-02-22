#include <EEPROM.h>

int num = 8;
int updateDelay = 100; //delay between each flap
uint8_t APins[8] = {A0,A1,A2,A3,A4,A5,A6,A7};
bool Bit[8] = {};
bool enable[8] = {};
int state[8] = {};
int indices[8] = {};
const int latch1 = 8;
const int clock1 = 13;
const int data1 = 12;
const int latch2 = 7;
const int clock2 = 11;
const int data2 = 10;
byte bitsToSendA = 0;
byte bitsToSendB = 0;

void setup() {
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
  pinMode(A6, INPUT_PULLUP);
  pinMode(A7, INPUT_PULLUP);

  
  Serial.begin(115200);
}

void loop() {
  Write("halloooo");
  delay(5000);
  Write("dag TIJL");
  delay(5000);
  Write(" hello  ");
  delay(1000);
  Write(" world  ");
  delay(5000);
  Write("testTEST");
  delay(5000);
}

bool isAllZero(int *arr){
  bool zero = 0;
  for (int i=0; i<num; i++){
    zero += arr[i];
  }
  return zero;
}

void setAll(int *arr, int to){
  for (int i=0; i<num; i++){
    arr[i] = to;
  }
}

void Write(String text){
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

  while (isAllZero(indices)){
    for (int i=0; i<num; i++){
      if (indices[i]){
        indices[i] -= 1;
        //digitalWrite(5-i, HIGH);
        registerWrite(i, !Bit[i]);
        Bit[i] = !Bit[i];
      }else{
        //digitalWrite(5-i, LOW);
      }
      delay(updateDelay/num);
    }
  }
}

//continues flapping until it reaches first segment
void Zero(){
  setAll(state, 1);
  while (isAllZero(state)){
    for (int i = 0; i<num; i++){
      delay(updateDelay/num);
      state[i] = digitalRead(APins[i]);
      if (state[i]){
        Serial.print(1);
        registerWrite(i, !Bit[i]);
        Bit[i] = !Bit[i];
      }else{
        Serial.print(1);
      }
    }
    Serial.print(" ");
  }
}


void registerWrite(int whichPin, int whichState) {
  digitalWrite(latch1, LOW);

  if (whichPin < 8){
    bitWrite(bitsToSendA, whichPin, whichState);
  }else{
    bitWrite(bitsToSendB, whichPin-8, whichState);
  }
  
  shiftOut(data1, clock1, MSBFIRST, bitsToSendB);
  shiftOut(data1, clock1, MSBFIRST, bitsToSendA);

  digitalWrite(latch1, HIGH);

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
    case '%':   output = 27;    break;
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
