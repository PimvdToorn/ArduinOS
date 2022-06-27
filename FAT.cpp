#include "FAT.hpp"


FATEntry FATClass::operator[] (uint8_t i) const{
    FATEntry entry;
    EEPROM.get(FATSTART + FATENTRYSIZE*i, entry);
    return entry;
}

// -----------------------------------------------------------------------

int8_t FATClass::addEntry(const char* name, const uint16_t* size, const uint16_t* address) const{
    
    if(size == 0 || (strcmp(name, "") == 0)) return -1;

    FATEntry FATEntry = {"", *size, *address};
    strcpy(FATEntry.name, name);

    EEPROM.put(FATSTART + FILECOUNT*FATENTRYSIZE, FATEntry);
    ++FILECOUNT;
    return 1;
}


bool FATClass::deleteEntry(const char* name) const{
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

// -----------------------------------------------------------------------


FATEntry FATClass::getFATEntry(const char* name) const{
    for(uint8_t i = 0; i < FILECOUNT; ++i){
        FATEntry file = FATClass::operator[](i);

        if(strcmp(name, file.name) == 0) return file;
    }
    return FATEntry{"",0,0};
}

uint8_t FATClass::getNameAddress(const char* name) const{
    for(uint8_t i = 0; i < FILECOUNT; ++i){
        uint8_t address = FATSTART + i*FATENTRYSIZE;

        char fileName[FILENAMELENGTH];
        EEPROM.get(address, fileName);

        if(strcmp(name, fileName) == 0) return address;
    }

    return 0;
}

uint16_t FATClass::getSize(const char* name) const{

    uint8_t address = getNameAddress(name);
    if(address == 0) return 0;

    uint16_t size;
    EEPROM.get(address + 12, size);

    return size; 
}

uint16_t FATClass::getAddress(const char* name) const{

    uint8_t address = getNameAddress(name);
    if(address == 0) return 0;

    uint16_t fileAddress;
    EEPROM.get(address + 12, fileAddress);

    return fileAddress; 
}





// -----------------------------------------------------------------------

int compare(const void *a, const void *b){
    /* (a > b) - (a < b) */
    return (*(uint16_t *)a > *(uint16_t *)b) - (*(uint16_t *)a < *(uint16_t *)b);
}

uint16_t FATClass::largestFreeSpace() const{
    // End of the last file and start of the next
    // Used to calculate the empty space between
    uint16_t endAddresses[FILECOUNT+1];
    uint16_t startAddresses[FILECOUNT+1];

    for (uint8_t i = 0; i < FILECOUNT; i++){
        FATEntry entry = FATClass::operator[](i);
        endAddresses[i] = entry.address + entry.size;
        startAddresses[i] = entry.address;
    }

    endAddresses[FILECOUNT] = FILESSTART;
    startAddresses[FILECOUNT] = EEPROMSIZE;

    qsort(endAddresses, FILECOUNT+1, sizeof(uint16_t), compare);
    qsort(startAddresses, FILECOUNT+1, sizeof(uint16_t), compare);

    uint16_t largest = 0;
    for(uint8_t i = 0; i < FILECOUNT+1; i++){
        largest = max(largest, startAddresses[i] - endAddresses[i]);
    }

    return largest;
}



uint16_t FATClass::totalFreeSpace() const{
    uint16_t freeSpace = EEPROMSIZE - FILESSTART;

    for(uint8_t i = 0; i < FILECOUNT; ++i){
        freeSpace -= FATClass::operator[](i).size;
    }

    return freeSpace;
}



uint16_t FATClass::getStoreAddress(uint16_t size) const{
    // End of the last file and start of the next
    // Used to calculate the empty space between
    uint16_t endAddresses[FILECOUNT+1];
    uint16_t startAddresses[FILECOUNT+1];

    for (uint8_t i = 0; i < FILECOUNT; i++){
        FATEntry entry = FATClass::operator[](i);
        endAddresses[i] = entry.address + entry.size;
        startAddresses[i] = entry.address;
    }

    endAddresses[FILECOUNT] = FILESSTART;
    startAddresses[FILECOUNT] = EEPROMSIZE;

    qsort(endAddresses, FILECOUNT+1, sizeof(uint16_t), compare);
    qsort(startAddresses, FILECOUNT+1, sizeof(uint16_t), compare);


    uint16_t address;
    uint16_t smallestSize = UINT16_MAX;

    for(uint8_t i = 0; i < FILECOUNT+1; i++){

        uint16_t newSize = startAddresses[i] - endAddresses[i];

        if(newSize >= size && newSize < smallestSize){
            smallestSize = newSize;
            address = endAddresses[i];
        }
    }

    return address;
}
