class Splitflap {
  public:
    Splitflap(int num);
    Splitflap(int numSegments, int updateDelayMs);
    void Zero();
    void WriteText(String text);
    void enableAll();
    void disableAll();
    void flipSegment(int segment);
    
  private:
    int num;
    int updateDelay;
    int state[32];
    int APins[32];
    bool Bit[32];
    int indices[32];
    int latch_enable;
    int clock_enable;
    int data_enable;
    int latchH;
    int clockH;
    int dataH;
    byte bitsToSendA;
    byte bitsToSendB;
    int lookup(char input, boolean red);
    void setAll(int *arr, int to, int len);
    bool isAllZero(int *arr);
    void writeIndices(int *indices);
    void writeSegment(int whichPin, int whichState);
    void enableSegment(int which, int whichState);
};

Splitflap::Splitflap(int numSegments){
  updateDelay = 75; //delay between each flap in milliseconds
  num = numSegments;

  APins[0] = A0;
  APins[1] = A1;
  APins[2] = A2;
  APins[3] = A3;
  APins[4] = A4;
  APins[5] = A5;
  APins[6] = 2;
  APins[7] = 3;
  
  latch_enable = 7;
  clock_enable = 11;
  data_enable = 10;
  
  latchH = 8;
  clockH = 13;
  dataH = 12;
  
  bitsToSendA = 0x00;
  bitsToSendB = 0xff;
};

Splitflap::Splitflap(int numSegments, int updateDelayMs){
  updateDelay = updateDelayMs; //delay between each flap in milliseconds
  num = numSegments;

  APins[0] = A0;
  APins[1] = A1;
  APins[2] = A2;
  APins[3] = A3;
  APins[4] = A4;
  APins[5] = A5;
  APins[6] = 2;
  APins[7] = 3;
  
  latch_enable = 7;
  clock_enable = 11;
  data_enable = 10;
  
  latchH = 8;
  clockH = 13;
  dataH = 12;
  
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



String incoming = "";
Splitflap splitflaps(8);

void setup() {
  const int latch1 = 7;
  const int clock1 = 11;
  const int data1 = 10;
  
  const int latch2 = 8;
  const int clock2 = 13;
  const int data2 = 12;
  
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
    splitflaps.enableAll();
    incoming = Serial.readString();
    incoming.remove(incoming.length()-1);//remove enter
    
    if (incoming.length() > 8){
      String a = "err: the string can't contain more than ";
      String b = " charakters.";
      Serial.println(a + 8 + b);
    } else {
      Serial.println(incoming);
      splitflaps.WriteText(incoming);
    }
    delay(300);
    splitflaps.disableAll();
  }
}
