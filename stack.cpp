#include "stack.hpp"

StackClass::StackClass(){
    stack = new byte[STACKSIZE];
}

StackClass::~StackClass(){
    delete stack;
}




int16_t StackClass::pushByte(const byte byte){
    if(stackPtr >= STACKSIZE) return STACKFULLERROR;

    stack[stackPtr++] = byte;
    return 1;
}

int16_t StackClass::pushChar(const char c){
    if(stackPtr >= STACKSIZE - 1) return STACKFULLERROR;

    pushByte(c);
    pushByte(CHAR);

    return 1;
}

int16_t StackClass::pushInt(const int16_t i){
    if(stackPtr >= STACKSIZE - 2) return STACKFULLERROR;
    
    pushByte(highByte(i));
    pushByte(lowByte(i));
    pushByte(INT);

    return 1;
}

int16_t StackClass::pushFloat(const float f){
    if(stackPtr >= STACKSIZE - 4) return STACKFULLERROR;

    byte* b = (byte*) &f;
    pushByte(b[3]);
    pushByte(b[2]);
    pushByte(b[1]);
    pushByte(b[0]);
    pushByte(FLOAT);

    return 1;
}

int16_t StackClass::pushString(char* s){
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




byte StackClass::peekByte(uint8_t offset = 0){
    if((stackPtr-1-offset) < 0) return NULL;
    
    return stack[stackPtr-1-offset];
}

char StackClass::peekChar(){
    if(stackPtr <= 1 || peekByte() != CHAR) return NULL;

    return peekByte(1);
}

int16_t StackClass::peekInt(){
    if(stackPtr <= 2 || peekByte() != INT) return NULL;

    int16_t i = peekByte(1);
    i += peekByte(2) << 8;
    return i;
}

float StackClass::peekFloat(){
    if(stackPtr <= 4 || peekByte() != FLOAT) return NULL;

    byte b[4];
    b[0] = peekByte(1);
    b[1] = peekByte(2);
    b[2] = peekByte(3);
    b[3] = peekByte(4);

    return *(float*) b;
}

float StackClass::peekVal(){

    switch (peekByte()){
    case CHAR:
        return peekChar();

    case INT:
        return peekInt();

    case FLOAT:
        return peekFloat();
    
    default:
        return NULL;
    }
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

int16_t StackClass::popExact(){
    switch (peekByte()){
    case CHAR:
        return popChar();

    case INT:
        return popInt();
    }
}

float StackClass::popVal(){

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

