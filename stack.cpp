#include "stack.hpp"

StackClass::StackClass(){
    stack = new byte[STACKSIZE];
}

StackClass::~StackClass(){
    delete stack;
}



byte StackClass::peekByte(uint8_t offset = 0){
    return stack[stackPtr-1-offset];
}

uint8_t StackClass::peekStrLen(){
    return peekByte(1);
}



int8_t StackClass::pushByte(byte byte){
    if(stackPtr >= STACKSIZE) return STACKFULLERROR;

    stack[stackPtr++] = byte;
    return 1;
}

int8_t StackClass::pushChar(char c){
    if(stackPtr >= STACKSIZE - 1) return STACKFULLERROR;

    pushByte(c);
    pushByte(CHAR);

    return 1;
}

int8_t StackClass::pushInt(int16_t i){
    if(stackPtr >= STACKSIZE - 2) return STACKFULLERROR;
    
    pushByte(highByte(i));
    pushByte(lowByte(i));
    pushByte(INT);

    return 1;
}

int8_t StackClass::pushFloat(float f){
    if(stackPtr >= STACKSIZE - 4) return STACKFULLERROR;

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
    if(stackPtr >= STACKSIZE - (len + 2)) return STACKFULLERROR;

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
    if(stackPtr <= 0) return NULL;

    return stack[--stackPtr];
}

char StackClass::popChar(){
    if(stackPtr <= 1 || peekByte() != CHAR) return NULL;
    popByte();

    return popByte();
}

int16_t StackClass::popInt(){
    if(stackPtr <= 2 || peekByte() != INT) return NULL;
    popByte();

    int16_t i = popByte();
    i += popByte() << 8;
    return i;
}

float StackClass::popFloat(){
    if(stackPtr <= 4 || peekByte() != FLOAT) return NULL;
    popByte();

    byte b[4];
    b[0] = popByte();
    b[1] = popByte();
    b[2] = popByte();
    b[3] = popByte();

    return *(float*) b;
}

char* StackClass::popString(char* s){
    if(stackPtr <= 2 || peekByte() != STRING) return NULL;
    popByte();

    uint8_t len = popByte();

    for(uint8_t i = len; i > 0; --i){
        s[i-1] = popByte();
    }

    return s;
}

float StackClass::popVal(){
    if(stackPtr <= 1) return NULL;

    switch (peekByte()){
    case CHAR:
        return popChar();

    case INT:
        return popInt();

    case FLOAT:
        return popFloat();
    
    default:
        return NULL;
    }
}

