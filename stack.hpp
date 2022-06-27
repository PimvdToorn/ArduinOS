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

        int16_t pushByte(const byte byte);
        int16_t pushChar(const char c);
        int16_t pushInt(const int16_t i);
        int16_t pushFloat(const float f);
        int16_t pushString(char* s);

        byte peekByte(uint8_t offset = 0);
        char peekChar();
        int16_t peekInt();
        float peekFloat();
        float peekVal();

        byte popByte();
        char popChar();
        int16_t popInt();
        float popFloat();
        char* popString(char* s);
        float popVal();

};