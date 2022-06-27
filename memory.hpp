#pragma once
#include "Arduino.h"
#include "instruction_set.h"
#include "errorCodes.hpp"
#include "procTable.hpp"

// #define SHAREDCHAR 255
// #define SHAREDINT 254
// #define SHAREDSTRING 253 
// #define SHAREDFLOAT 252


typedef struct variable{
    uint8_t id;
    char name;
    uint8_t type;
    uint8_t address;
};

class MemoryClass{
    private:
        uint16_t* memSize;
        uint8_t* varTableSize;

        byte* memory;
        variable* varTable;
        uint8_t* nVariables;

        process* procTable;
        
        int16_t freeSpace(const uint8_t size);

    public:
        MemoryClass(byte* memory, variable* varTable, uint8_t* nVariables, uint16_t* memSize, uint8_t* varTableSize, process* procTable);
        

        variable* getVar(const char name, const uint8_t id);
        int16_t eraseVar(const char name, const uint8_t id);
        int16_t eraseAll(const uint8_t id);


        int16_t set(const char name, const uint8_t id);
        int16_t get(const char name, const uint8_t id);

};
