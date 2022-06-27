#pragma once
#include "Arduino.h"
#include "instruction_set.h"
#include "errorCodes.hpp"

#define STACKSIZE 32

// #define POP --stackPtr
// #define PEEK stack[stackPtr-1]

class StackClass{
    private:
        byte* stack;
        uint8_t stackPtr = 0;

    public:
        StackClass();
        ~StackClass();

        byte peekByte(uint8_t offset = 0);
        uint8_t peekStrLen();

        int8_t pushByte(byte byte);
        int8_t pushChar(char c);
        int8_t pushInt(int16_t i);
        int8_t pushFloat(float f);
        int8_t pushString(char* s);

        byte popByte();
        char popChar();
        int16_t popInt();
        float popFloat();
        char* popString(char* s);
        float popVal();

};