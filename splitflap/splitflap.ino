//int numDisplays = 3;
int updateDelay = 200;
byte Bit = false;

void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Update(1000);
}

void Update(int flapDelay){
  digitalWrite(12, Bit);
  digitalWrite(13, Bit);
  delay(flapDelay);
  Bit = !Bit;
}

void Zero(){
  while(digitalRead(A0)){
    Update(updateDelay);
  }
}

void increase(int num){
  if (num < 54){
    for (int i=0; i<num; i++){
      Update(updateDelay);
    }
  }else{
    //error code: reached segment limit
  }
}

/*void drawChar(int displ, int state){
  int flapDelay = 100;
  
  if (state < prevState){
    while(hallSensor == 0){
      Update();
    }
    prevState = 0;
  }
  
  for (int i = prevState; i<state; i++){
    Update();
  }
}*/
