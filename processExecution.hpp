#pragma once
#include "Arduino.h"
#include "eeprom.h"
#include "errorCodes.hpp"
#include "instruction_set.h"

#include "FAT.hpp"
#include "procTable.hpp"
#include "memory.hpp"

#define PC procTable[id].data->pc
#define pcREAD EEPROM[procTable[id].data->pc++]
#define pStack procTable[id].data->stack
#define LR procTable[id].data->lr
#define FP procTable[id].data->fp
#define ST procTable[id].status

class processExecution{
    private:
        process* procTable;
        MemoryClass* memory;
        ProcTableClass* ptc;

        int16_t unaryOperators(const uint8_t id, const byte uOperator);
        int16_t binaryOperators(const uint8_t id, const byte bOperator);

        int16_t print(const uint8_t id);
        int16_t write(const uint8_t id);

    public:
        processExecution(process* procTable, MemoryClass* memory, ProcTableClass* ptc);

        int16_t executeInstruction(const uint8_t id);


};
