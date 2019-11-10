#include <EEPROM.h>

int updateDelay = 60; //delay between each flap
uint8_t APins[4] = {A0,A1,A2,A3};
bool Bit[4] = {false,false,false,true}; //used by "Update" function
bool state[4] = {1,1,1,1};

//String message = "dag tijl";

void setup() {
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  
  Serial.begin(115200);
  
}

void loop() {
  String text = "text";
  int indices[4] = {};

  Zero();
  for (int i=0; i<4; i++){
    indices[i] = lookup(text[i],false);
  }

  for (int i=0; i<text.length(); i++){
    for (int i = 0; i<4; i++){
      if (!indices[3-i]){
        digitalWrite(5-i, 1);
        digitalWrite(13-i*2, Bit[i]);
        digitalWrite(12-i*2, !Bit[i]);
        Bit[i] = !Bit[i];
      }
      Serial.print(state[i]);
    }
  }
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

//jump to a character on the display
/*void jumpTo(int Display, int num){
  int prev = EEPROM.read(Display);
  bool state[4] = {0,0,0,0};
  
  if (num < prev){
    Zero(Display);
    increase(Display,num);
  }
  
  else if (num > prev){
    for (int i=prev; i<num;i++){
      Update(Display,state);
    }
  }
  EEPROM.write(Display,num);
}*/

//flaps until it reaches the first display-segment
void Zero(){
  bool state[4] = {1,1,1,1};
  while ((state[0]||state[1]||state[2]||state[3])){
    for (int i = 0; i<4; i++){
      state[i] = digitalRead(APins[3-i]);
    }
    for (int i = 0; i<4; i++){
      
      if (state[3-i]){
        digitalWrite(5-i, 1);
        digitalWrite(13-i*2, Bit[i]);
        digitalWrite(12-i*2, !Bit[i]);
        Bit[i] = !Bit[i];
      }
    }
    delay(updateDelay);
    for (int i = 0; i<4; i++){
      digitalWrite(2+i, 0);
    }
  }
}

//flaps a certain amount of times
/*void increase(int Display, int num){
  //int prev = EEPROM.read(Display);
  //if (num+prev < 60){
    
    for (int i=0; i<num; i++){
      bool state[4] = {0,0,0,0};
      Update(state);
    }
    
    //EEPROM.write(Display, (num+prev)%60);
  //}else{
    //error code: reached segment limit
  //}
}*/
