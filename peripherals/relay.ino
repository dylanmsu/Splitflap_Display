int testa[4] = {A1,A2,A3,A4};

String input = "0111";


int test_heat = input[0] - '0';
int test_vent = input[1] - '0';
int test_wifi = input[2] - '0';
int test_light = input[3] - '0';

void setup() {
  
  for(int i=0;i<4;i++){
    pinMode(testa[i], OUTPUT); 
  }
}

void loop() {
if (test_heat == 1){
 digitalWrite(testa[0], LOW); 
} else {
 digitalWrite(testa[0], HIGH);  
}

if (test_vent == 1){
 digitalWrite(testa[1], LOW); 
} else {
 digitalWrite(testa[1], HIGH);  
}

  if (test_wifi == 1){
 digitalWrite(testa[2], LOW); 
} else {
 digitalWrite(testa[2], HIGH);  
}

if (test_light == 1){
 digitalWrite(testa[3], LOW); 
} else {
 digitalWrite(testa[3], HIGH);  
}
}
