#include "memory.hpp"


MemoryClass::MemoryClass(byte* memory, variable* varTable, uint8_t* nVariables, uint16_t* memSize, uint8_t* varTableSize, process* procTable):
    memory (memory), varTable (varTable), nVariables (nVariables), memSize (memSize), varTableSize (varTableSize), procTable (procTable){}




variable* MemoryClass::getVar(const char name, const uint8_t id){

    for(uint8_t i = 0; i < *nVariables; i++){

        if(varTable[i].id == id && varTable[i].name == name){
            return &varTable[i];
        }
    }

    return nullptr;
}

int16_t MemoryClass::eraseVar(const char name, const uint8_t id){
    for(uint8_t i = 0; i < *nVariables; i++){

        if(varTable[i].id == id && varTable[i].name == name){

            // replace the var with the last and decrement nVar in the process
            varTable[i] = varTable[--(*nVariables)];

            return 1;
        }
    }

    return NOTFOUND;
}

int16_t MemoryClass::eraseAll(const uint8_t id){
    int16_t erased = NOTFOUND;

    for(uint8_t i = 0; i < *nVariables; i++){

        if(varTable[i].id == id){

            // replace the var with the last and decrement nVar in the process
            varTable[i] = varTable[--(*nVariables)];
            
            // Decrement so the new var at i gets checked
            i--;

            erased = 1;
        }
    }

    return erased;
}



int compare8(const void *a, const void *b){
    /* (a > b) - (a < b) */
    return (*(uint8_t *)a > *(uint8_t *)b) - (*(uint8_t *)a < *(uint8_t *)b);
}

int compare16(const void *a, const void *b){
    /* (a > b) - (a < b) */
    return (*(uint16_t *)a > *(uint16_t *)b) - (*(uint16_t *)a < *(uint16_t *)b);
}

int16_t MemoryClass::freeSpace(const uint8_t size){
    // End of the last var and start of the next
    // Used to calculate the empty space between
    uint8_t endAddresses[*nVariables+1];
    uint16_t startAddresses[*nVariables+1];

    for (uint8_t i = 0; i < *nVariables; i++){
        uint8_t thisSize = varTable[i].type == STRING ? memory[varTable[i].address] : varTable[i].type;
        endAddresses[i] = varTable[i].address + thisSize;
        startAddresses[i] = varTable[i].address;
    }

    endAddresses[*nVariables] = 0;
    startAddresses[*nVariables] = *memSize;


    qsort(endAddresses, *nVariables+1, sizeof(uint8_t), compare8);
    qsort(startAddresses, *nVariables+1, sizeof(uint16_t), compare16);


    int16_t address = MEMFULLERROR;
    uint8_t smallestSize = UINT8_MAX;


    for(uint8_t i = 0; i < *nVariables+1; i++){

        uint8_t newSize = startAddresses[i] - endAddresses[i];

        if(newSize >= size && newSize < smallestSize){
            smallestSize = newSize;
            address = endAddresses[i];
        }
    }

    
    return address;
}

int16_t MemoryClass::set(const char name, const uint8_t id){
    if(procTable[id].status == '\0') return NOTFOUND;
    if(procTable[id].status == 't') return TERMINATED;

    variable* var = getVar(name, id);
    
    // type change or not found
    if(var == nullptr || var->type != stack(id).peekByte()) {
        
        if(var != nullptr) eraseVar(name, id); // if found, erase

        else if(*nVariables >= *varTableSize) return VARTABLEFULLERROR;

        var = &(varTable[*nVariables] = {
            id, 
            name, 
            stack(id).popByte(), 
            0
        });
        (*nVariables)++;
        
        int16_t address = 0;
        if(var->type != STRING) address = freeSpace(var->type); // set later for string

        if(address == MEMFULLERROR) return MEMFULLERROR;
        var->address = (uint8_t) address;
    }

    else stack(id).popByte();




    if(var->type == STRING){

        uint8_t size = stack(id).popByte();

        // if no address or size has changed, get new location (if string is same size, but the actual address is 0, it looks for new space anyway)
        if(var->address == 0 || size != memory[var->address]){

            memory[var->address] = 0; // set size to 0 to free current space
            
            int16_t address = freeSpace(size + 1); // +1 to store size 
            if(address == MEMFULLERROR) return MEMFULLERROR; 
            var->address = (uint8_t) address;
        }
        

        for(int8_t i = 1; i < size + 1; i++){
            memory[var->address + i] = stack(id).popByte();
        }

    }


    else{   // Char, Int or Float

        for(int8_t i = 0; i < var->type; i++){
            memory[var->address + i] = stack(id).popByte();
        }
        
    }

}

int16_t MemoryClass::get(const char name, const uint8_t id){
    if(procTable[id].status == '\0') return NOTFOUND;
    if(procTable[id].status == 't') return TERMINATED;

    variable* var = getVar(name, id);
    if(var == nullptr) return NOTFOUND;



    if(var->type == STRING){

        uint8_t size = memory[var->address];

        for(uint8_t i = var->address + size; i > var->address; i--){

            if(stack(id).pushByte(memory[i]) == STACKFULLERROR) return STACKFULLERROR;

        }

        if(stack(id).pushByte(size) == STACKFULLERROR) return STACKFULLERROR;
        if(stack(id).pushByte(STRING) == STACKFULLERROR) return STACKFULLERROR;

    }


    else{

        for(uint8_t i = var->address + var->type - 1; i >= var->address; i--){

            if(stack(id).pushByte(memory[i]) == STACKFULLERROR) return STACKFULLERROR;

        }

        if(stack(id).pushByte(var->type) == STACKFULLERROR) return STACKFULLERROR;
        
    }

    return 1;
}