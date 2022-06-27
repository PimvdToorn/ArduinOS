#pragma once
#include "Arduino.h"
#include "stack.hpp"
#include "FAT.hpp"
#include "errorCodes.hpp"

#define stack(a) procTable[a].data->stack

typedef struct processData{
    uint16_t pc;    // Program counter
    uint16_t fp;    // File pointer
    uint16_t lr;    // Loop register
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
        uint8_t* maxProcesses; 
        uint8_t* maxActive;

        process* procTable;
        uint8_t* nActiveProcesses;
        uint8_t* nProcesses;


    public:
        ProcTableClass(process* procTable, uint8_t* nActiveProcesses, uint8_t* nProcesses, uint8_t* maxProcesses, uint8_t* maxActive);

        int8_t getProcessName(const uint8_t id,char* namePtr);

        int8_t addProcess(char* name);
        int8_t terminateProcess(uint8_t id);
        int8_t suspendProcess(uint8_t id);
        int8_t resumeProcess(uint8_t id);

  
};



