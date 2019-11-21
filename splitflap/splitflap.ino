#include <EEPROM.h>

int num = 8;
int updateDelay = 100; //delay between each flap
uint8_t APins[8] = {A0,A1,A2,A3,A4,A5,2,3};
bool Bit[8] = {};
bool enable[8] = {};
bool state[8] = {};
int indices[8] = {};
const int latchPin = 8;
const int clockPin = 12;
const int dataPin = 11;
byte bitsToSendA = 0;
byte bitsToSendB = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  
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

void Write(String text){
  Zero();
  String temp = text;
  text.toLowerCase();
  for (int i=0; i<text.length(); i++){
    if (temp[i] != text[i]){
      indices[i] = lookup(text[i],true);
    }else{
      indices[i] = lookup(text[i],false);
    }
  }
<<<<<<< HEAD

  while (indices[0]||indices[1]||indices[2]||indices[3]||indices[4]||indices[5]||indices[6]||indices[7]){
    for (int i=0; i<num; i++){
=======
  
  while (indices[0]||indices[1]||indices[2]||indices[3]){
    for (int i=0; i<4; i++){
>>>>>>> 60bf69825c6ccf469be435b1e517519a25aeaaff
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

//flaps until it reaches the first display-segment
void Zero(){
  bool state[8] = {1,1,1,1,1,1,1,1};
  while ((state[0]||state[1]||state[2]||state[3]||state[4]||state[5]||state[6]||state[7])){
    for (int i = 0; i<num; i++){
      delay(updateDelay/num);
      state[i] = digitalRead(APins[i]);
      if (state[i]){
        registerWrite(i, !Bit[i]);
        Bit[i] = !Bit[i];
      }else{
      }
    }
  }
}

void registerWrite(int whichPin, int whichState) {
  digitalWrite(latchPin, LOW);

  if (whichPin < 8){
    bitWrite(bitsToSendA, whichPin, whichState);
  }else{
    bitWrite(bitsToSendB, whichPin-8, whichState);
  }
  
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSendB);
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSendA);

  digitalWrite(latchPin, HIGH);

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
  output = 0;
}
