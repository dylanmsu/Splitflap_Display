#include "arduino.h"
#include "Splitflap.h"


Splitflap::Splitflap(int *sensPins,  int updateDelayMs, int *serialPins){
    updateDelay = updateDelayMs; //delay between each flap in milliseconds

    for (int i=0; i<32; i++){
        APins[i] = sensPins[i];
        if (sensPins[i] != 0){
            pinMode(APins[i], INPUT);
        }
    }

    setAll(Bit, 0, 32);
    
    latch_enable = serialPins[4];
    clock_enable = serialPins[2];
    data_enable = serialPins[0];
    
    latchH = serialPins[5];
    clockH = serialPins[3];
    dataH = serialPins[1];


    for (int i=0; i<6; i++){
        pinMode(serialPins[i], OUTPUT);
    }
    
    stateSegment = 0x00000000;
    stateEnable = 0x00000000;
};

bool Splitflap::Send(String text, int icon_index, int hours, int minutes)
{
    if (text.length() <= 28 && hours <= 23 && hours >= 0 && minutes <= 59 && minutes >= 0){
        enableAll();

        int indices[32] = {};
        String temp = text;
        text.toLowerCase();
        for (int i=0; i<text.length(); i++){
            if (temp[i] != text[i]){
                indices[i] = lookup(text[i],true);
            }else{
                indices[i] = lookup(text[i],false);
            }
            //Serial.print(String(indices[i]) + " ");
        }

        indices[28] = icon_index;

        if (hours == 0){
            indices[29] = 30;
        } else {
            indices[29] = hours;
        }

        int minutesA = minutes%10;
        int minutesB = (minutes/10)%10;

        if (minutesB == 0){
            indices[30] = 10;
        } else {
            indices[30] = minutesB;
        }

        if (minutesA == 0){
            indices[31] = 10;
        } else {
            indices[31] = minutesA;
        }
        

        delay(updateDelay);

        int hasBeenZerod[32] = {};
        int flapProgress[32] = {};

        for (int i=0; i<32; i++) {
            if (APins[i] != 0){
                hasBeenZerod[i] = digitalRead(APins[i]);
                flapProgress[i] = 0;
            }
        }

        while (stopflapping(indices, flapProgress)) {
            for (int i=0; i<32; i++)  {
                if (currentIndices[i] == NULL || currentIndices[i] != indices[i]) {
                    // flap to zero and then flap until it reaches the desired position if the new position is smaller than the current position

                    // flap to the desired position if it has been zeord
                    if (hasBeenZerod[i] && (indices[i] - (flapProgress[i])) > 0) {
                        flapProgress[i] += 1;
                        flipSegment(i);
                    }

                    // flap to zero
                    if (!digitalRead(APins[i]) && !hasBeenZerod[i]){
                        flipSegment(i);
                        hasBeenZerod[i] = 0;
                    } else {
                        hasBeenZerod[i] = 1;
                    }
                } else {
                    flapProgress[i] = indices[i];
                }

                delay(updateDelay/32);
            }
        }

        for (int i=0; i<32; i++) {
            currentIndices[i] = indices[i];
        }

        disableAll();
        return true;
    } else {
        Serial.println("ERR: trying to display "+String(text.length())+" charakters, but you can only display 28");
        return false;
    }
};

bool Splitflap::stopflapping(int *a, int *b) {
     bool zero = 0;
    for (int i=0; i<32; i++){
        zero += a[i] - b[i];
    }
    return zero;
}

//continues flapping until all segments are on the 0'th position
void Splitflap::ResetAll(){
    setAll(State, 0, 32);
    while (!isAllOne(State)){
        for (int i = 0; i<32; i++){
            delay(updateDelay/32);
            State[i] = digitalRead(APins[i]);
            if (!State[i]){
                flipSegment(i);
            }
        }
    }
    for (int i=0; i<32; i++) {
        currentIndices[i] = 0;
    }
    delay(updateDelay);
};

void Splitflap::flipSegment(int segment){
    writeSegment(segment, !Bit[segment]);
    Bit[segment] = !Bit[segment];
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

bool Splitflap::isAllOne(int *arr){
    bool zero = 0;
    for (int i=0; i<32; i++){
        zero += !arr[i];
    }
    return zero == 0;
};

void Splitflap::writeSegment(int whichPin, int whichState) {
    digitalWrite(latchH, LOW);

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
    
    bitWrite(stateEnable,whichPin,whichState);
    
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
