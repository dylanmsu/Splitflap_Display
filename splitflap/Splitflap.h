#ifndef Splitflap_h
#define Splitflap_h

#include "arduino.h"

class Splitflap {
  public:
    Splitflap(int numSegments, int *sensPins,  int updateDelayMs, int *serialPins);
    void Zero();
    void WriteText(String text);
    void enableAll();
    void disableAll();
    void flipSegment(int segment);
    void writeEnable(int which, int whichState);
    void writeSegment(int whichPin, int whichState);
    
  private:
    int num;
    int updateDelay;
    int state[32];
    int APins[32];
    int Bit[32];
    int indices[32];
    int latch_enable;
    int clock_enable;
    int data_enable;
    int latchH;
    int clockH;
    int dataH;
    uint16_t stateSegment;
    uint16_t stateEnable;
    int lookup(char input, boolean red);
    void setAll(int *arr, int to, int len);
    bool isAllZero(int *arr);
    void writeIndices(int *indices);
};

#endif
