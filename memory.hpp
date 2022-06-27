#pragma once
#include "Arduino.h"
#include "instruction_set.h"

#define SHAREDCHAR 255
#define SHAREDINT 254
#define SHAREDSTRING 253 
#define SHAREDFLOAT 252

#define VARTABLESIZE 64

typedef struct variable{
    char name;
    uint8_t type;
    uint8_t size;
    uint8_t address;
    uint8_t id;
};

class MemoryClass{
    private:

    public:
        byte memory[256];
        variable varTable[VARTABLESIZE];
        uint8_t nVariables = 0;

};
static MemoryClass mem;