#include "stack.hpp"

int8_t StackClass::pushByte(byte byte){
    if(stackPtr >= STACKSIZE) return 0;

    stack[stackPtr] = byte;
    stackPtr++;
    return 1;
}

int8_t StackClass::pushChar(char c){
    if(stackPtr >= STACKSIZE - 1) return 0;

    pushByte(c);
    pushByte(CHAR);

    return 1;
}

int8_t StackClass::pushInt(int16_t i){
    if(stackPtr >= STACKSIZE - 2) return 0;

    pushByte(highByte(i));
    pushByte(lowByte(i));
    pushByte(INT);

    return 1;
}

int8_t StackClass::pushFloat(float f){
    if(stackPtr >= STACKSIZE - 4) return 0;

    byte* b = (byte*) &f;
    pushByte(b[3]);
    pushByte(b[2]);
    pushByte(b[1]);
    pushByte(b[0]);
    pushByte(FLOAT);

    return 1;
}

int8_t StackClass::pushString(char* s){
    uint8_t len = strlen(s) + 1;
    if(stackPtr >= STACKSIZE - (len + 2)) return 0;

    s--;
    do{
        s++;
        pushByte(*s);
    } while(*s != '\0');

    pushByte(len);
    pushByte(STRING);

    return 1;
}


byte StackClass::popByte(){

}

int16_t StackClass::popInt(){

}

float StackClass::popFloat(){

}

char* StackClass::popString(){

}

float StackClass::popVal(){

}

