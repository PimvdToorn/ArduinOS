#pragma once
#include "Arduino.h"
#include "stack.hpp"
#include "FAT.hpp"
#include "errorCodes.h"

// #define PNAMELENGTH 12
#define MAXPROCESSES 32
#define MAXACTIVE 10



typedef struct processData{
    uint16_t pc;    // Program counter
    uint16_t fp;    // File pointer
    StackClass stack;
};

typedef struct process{
    uint8_t nameAddress;
    char status;
    processData* data;

    process(){
        nameAddress = 0;
        status = '\0';
        data = NULL;
    }
    process(const uint8_t nameAddress, const char status, processData* data)
        : nameAddress(nameAddress), status(status), data(data){}
};

class ProcTableClass{
    private:

    public:
        process procTable[MAXPROCESSES];
        uint8_t nActiveProcesses = 0;
        uint8_t nProcesses = 0;

        int8_t getProcessName(const uint8_t id,char* namePtr);

        int8_t addProcess(char* name);
        int8_t terminateProcess(uint8_t id);
        int8_t suspendProcess(uint8_t id);
        int8_t resumeProcess(uint8_t id);

  
};
static ProcTableClass pT;


