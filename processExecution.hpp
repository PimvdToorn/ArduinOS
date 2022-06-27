#pragma once
#include "Arduino.h"
#include "eeprom.h"
#include "errorCodes.hpp"
#include "instruction_set.h"

#include "procTable.hpp"
#include "memory.hpp"

#define PC procTable[id].data->pc
#define pcREAD EEPROM[procTable[id].data->pc++]
#define pStack procTable[id].data->stack
#define LR procTable[id].data->lr

class processExecution{
    private:
        process* procTable;
        MemoryClass* memory;

        int16_t unaryOperators(const uint8_t id, const byte uOperator);
        int16_t binaryOperators(const uint8_t id, const byte bOperator);

    public:
        processExecution(process* procTable, MemoryClass* memory);

        int16_t executeInstruction(const uint8_t id);


};
