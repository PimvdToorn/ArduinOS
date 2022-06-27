#pragma once
#include "Arduino.h"
#include "instruction_set.h"

#define STACKSIZE 32

class StackClass{
    private:
        byte stack[STACKSIZE];
        uint8_t stackPtr = 0;

    public:

        int8_t pushByte(byte byte);
        int8_t pushChar(char c);
        int8_t pushInt(int16_t i);
        int8_t pushFloat(float f);
        int8_t pushString(char* s);

        byte popByte();
        int16_t popInt();
        float popFloat();
        char* popString();
        float popVal();

};