#define DisplayA
#define DisplayB
#define DisplayC

int numDisplays = 3;

void setup() {
  pinMode(DislpayA, OUTPUT);
  pinMode(DislpayB, OUTPUT);
  pinMode(DislpayC, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void drawChar(int displ, int state){
  int flapDelay = 100;
  /*
   * if (state < prevState){
   *   while(hallSensor == 0){
   *     digitalWrite(DisplayA, 1);
   *     delay(flapDelay/2);
   *     digitalWrite(DisplayA, 0);
   *     delay(flapDelay/2);
   *   }
   *   prevState = 0;
   * }
   * 
   * for (int i = prevState; i<state; i++){
   *   digitalWrite(DisplayA, 1);
   *   delay(flapDelay/2);
   *   digitalWrite(DisplayA, 0);
   *   delay(flapDelay/2);
   * }
   */
}
