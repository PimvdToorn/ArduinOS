#include "procTable.hpp"

int8_t ProcTableClass::getProcessName(const uint8_t id, char* namePtr){
    if(procTable[id].status == '\0') return -1;

    char name[FILENAMELENGTH];
    EEPROM.get(procTable[id].nameAddress, name);
    strcpy(namePtr, name);

    return 1;
}

int8_t ProcTableClass::addProcess(char* name){
    if(nActiveProcesses >= MAXACTIVE) return MAXACTIVEERROR;
    if(nProcesses >= MAXPROCESSES) return MAXTOTALERROR;
    if(FAT.getNameAddress(name) == 0) return NOTFOUND;

    procTable[nProcesses] = process{
        FAT.getNameAddress(name),
        'r',
        new processData{
            FAT.getAddress(name),
            0,
        }
    };

    nProcesses++;
    nActiveProcesses++;
    return 1;
}


int8_t ProcTableClass::terminateProcess(uint8_t id){
    if(procTable[id].status == '\0') return NOTFOUND;
    if(procTable[id].status == 't') return TERMINATED;

    delete procTable[id].data;
    procTable[id].status = 't';
    return TERMINATED;
}

int8_t ProcTableClass::suspendProcess(uint8_t id){
    if(procTable[id].status == '\0') return NOTFOUND;
    if(procTable[id].status == 't') return TERMINATED;

    procTable[id].status = 's';
    return 1;
}

int8_t ProcTableClass::resumeProcess(uint8_t id){
    if(procTable[id].status == '\0') return NOTFOUND;
    if(procTable[id].status == 't') return TERMINATED;

    procTable[id].status = 'r';
    return 1;
}