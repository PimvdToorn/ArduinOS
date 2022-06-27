#include "FAT.hpp"


FATEntry FATClass::operator[] (uint8_t i) const{
    FATEntry entry;
    EEPROM.get(FATSTART + FATENTRYSIZE*i, entry);
    return entry;
}



void FATClass::addEntry(const char* name, const uint16_t* size, const uint16_t* address){
    FATEntry FATEntry = {"", *size, *address};
    strcpy(FATEntry.name, name);

    EEPROM.put(FATSTART + FILECOUNT*FATENTRYSIZE, FATEntry);
    ++FILECOUNT;
}


bool FATClass::deleteEntry(char* name){
    for(uint8_t i = 0; i < FILECOUNT; ++i){
        FATEntry file = FATClass::operator[](i);

        if(strcmp(name, file.name) == 0){
            EEPROM.put(FATSTART + FATENTRYSIZE*i, FATClass::operator[](FILECOUNT-1));

            --FILECOUNT;
            return true;
        }
    }
    return false;
}



FATEntry FATClass::findFile(char* name){
    for(uint8_t i = 0; i < FILECOUNT; ++i){
        FATEntry file = FATClass::operator[](i);

        if(strcmp(name, file.name) == 0) return file;
    }
    return FATEntry{"",0,0};
}



int compare(const void *a, const void *b){
    /* (a > b) - (a < b) */
    return (*(uint16_t *)a > *(uint16_t *)b) - (*(uint16_t *)a < *(uint16_t *)b);
}

uint16_t FATClass::largestFreeSpace(){
    uint16_t startAddresses[FILECOUNT+1];
    uint16_t endAddresses[FILECOUNT+1];

    for (uint8_t i = 0; i < FILECOUNT; i++){
        FATEntry entry = FATClass::operator[](i);
        startAddresses[i] = entry.address;
        endAddresses[i] = entry.address + entry.size;
    }

    startAddresses[FILECOUNT] = EEPROMSIZE;
    endAddresses[FILECOUNT] = FILESSTART;

    qsort(startAddresses, FILECOUNT+1, sizeof(uint16_t), compare);
    qsort(endAddresses, FILECOUNT+1, sizeof(uint16_t), compare);

    uint16_t largest = 0;
    for(uint8_t i = 0; i < FILECOUNT+1; i++){
        largest = max(largest, startAddresses[i] - endAddresses[i]);
    }

    return largest;
}




uint16_t FATClass::totalFreeSpace(){
    uint16_t freeSpace = EEPROMSIZE - FILESSTART;

    for(uint8_t i = 0; i < FILECOUNT; ++i){
        freeSpace -= FATClass::operator[](i).size;
    }

    return freeSpace;
}



uint16_t FATClass::getStoreAddress(uint16_t size){
    uint16_t startAddresses[FILECOUNT+1];
    uint16_t endAddresses[FILECOUNT+1];

    for (uint8_t i = 0; i < FILECOUNT; i++){
        FATEntry entry = FATClass::operator[](i);
        startAddresses[i] = entry.address;
        endAddresses[i] = entry.address + entry.size;
    }

    startAddresses[FILECOUNT] = EEPROMSIZE;
    endAddresses[FILECOUNT] = FILESSTART;

    qsort(startAddresses, FILECOUNT+1, sizeof(uint16_t), compare);
    qsort(endAddresses, FILECOUNT+1, sizeof(uint16_t), compare);


    uint16_t address;
    uint16_t smallestSize;

    for(uint8_t i = 0; i < FILECOUNT+1; i++){

        uint16_t newSize = startAddresses[i] - endAddresses[i];

        if(newSize >= size && newSize < smallestSize){
            smallestSize = newSize;
            address = endAddresses[i];
        }
    }

    return address;
}
