#ifndef Splitflap_h
#define Splitflap_h

#include "Arduino.h"

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

#endif