#ifndef Splitflap_h
#define Splitflap_h

#include "Arduino.h"

class Splitflap {
    public:
        Splitflap(int *sensPins,  int updateDelayMs, int *serialPins);
        void writeSegment(int whichPin, int whichState);
        bool Send(String text, int hours, int minutes);
        void writeEnable(int which, int whichState);
        void writeIndices(int *indices);
        void flipSegment(int segment);
        bool WriteText(String text);
        void disableAll();
        void enableAll();
        void ResetAll();
        
    private:
        void setAll(int *arr, int to, int len);
        int lookup(char input, boolean red);
        bool stopflapping(int *a, int *b);
        bool isAllZero(int *arr);
        int currentIndices[32];
        uint32_t stateSegment;
        uint32_t stateEnable;
        int latch_enable;
        int clock_enable;
        int data_enable;
        int updateDelay;
        int state[32];
        int APins[32];
        int Bit[32];
        int latchH;
        int clockH;
        int dataH;
        int num;
};

#endif
