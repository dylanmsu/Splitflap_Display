const int num = 8; //number of display segments
const int updateDelay = 75; //delay between each flap in milliseconds
uint8_t APins[num] = {A0,A1,A2,A3,A4,A5,2,3}; //the inputpins for the positionsensors of the splitflaps

bool Bit[num] = {}; //keeps track of the current state of the toggelers
bool enable[num] = {};
int state[num] = {};
int indices[num] = {};

const int latch1 = 8;
const int clock1 = 13;
const int data1 = 12;

const int latch2 = 7;
const int clock2 = 11;
const int data2 = 10;

byte bitsToSendA = 0x00;
byte bitsToSendB = 0xff;

String incoming = "        ";

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
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    enableAll();
    incoming = Serial.readString();
    incoming.remove(incoming.length()-1);//remove enter
    if (incoming.length() > num){
      String a = "err: the string can't contain more than ";
      String b = " charakters.";
      Serial.println(a + num + b);
    } else {
      Serial.println(incoming);
      Write(incoming);
    }
    delay(300);
    disableAll();
  } 
}

void enableAll(){
  digitalWrite(latch1, LOW);
  shiftOut(data1, clock1, MSBFIRST, 0xFF); //set all enable pins to high 
  digitalWrite(latch1, HIGH);
}

void disableAll(){
  digitalWrite(latch1, LOW);
  shiftOut(data1, clock1, MSBFIRST, 0x00); //set all enable pins to low
  digitalWrite(latch1, HIGH);
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
      indices[(num-1)-i] = lookup(text[i],true);
    }else{
      indices[(num-1)-i] = lookup(text[i],false);
    }
  }

  while (isAllZero(indices)){
    for (int i=0; i<num; i++){
      if (indices[i]){
        indices[i] -= 1;
        //enableSegment(i, HIGH);
        writeSegment(i, !Bit[i]);
        Bit[i] = !Bit[i];
      }else{
        //enableSegment(i, LOW);
      }
      delay(updateDelay/num);
    }
  }
}

//continues flapping until all segments are on the 0'th position
void Zero(){
  setAll(state, 1);
  while (isAllZero(state)){
    for (int i = 0; i<num; i++){
      delay(updateDelay/num);
      state[i] = digitalRead(APins[i]);
      if (state[i]){
        writeSegment(i, !Bit[i]);
        Bit[i] = !Bit[i];
      }else{
      }
    }
  }
}


void writeSegment(int whichPin, int whichState) {
  digitalWrite(latch2, LOW);

  bitWrite(bitsToSendA, whichPin, whichState);
  shiftOut(data2, clock2, MSBFIRST, bitsToSendA);

  digitalWrite(latch2, HIGH);
}

void enableSegment(int which, int whichState) {
  digitalWrite(latch1, LOW);

  bitWrite(bitsToSendB, which, whichState);
  shiftOut(data1, clock1, MSBFIRST, bitsToSendB);  

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
