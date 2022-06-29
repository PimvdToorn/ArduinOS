#pragma once
#include "Arduino.h"
#include "EEPROM.h"
#include "errorCodes.hpp"

#define EEPROMSIZE 1024

#define FATSIZE 10
#define FATENTRYSIZE 16
#define FATSTART 1

#define FILENAMELENGTH 12
#define FILESIZEDIGITS 5
#define FILESSTART (FATSTART + FATSIZE*FATENTRYSIZE)
#define FILECOUNT EEPROM[0]


typedef struct FATEntry{
    char name[FILENAMELENGTH]; 
    uint16_t size;
    uint16_t address;
};


class FATClass{
    private:

    public: 

        FATEntry operator [] (uint8_t i) const;

        int16_t addEntry(const char* name, const uint16_t* size, const uint16_t* address) const; 
        bool deleteEntry(const char* name) const;

        FATEntry getFATEntry(const char* name) const;
        uint8_t getNameAddress(const char* name) const;
        uint16_t getSize(const char* name) const;
        uint16_t getAddress(const char* name) const;

        uint16_t largestFreeSpace() const;
        uint16_t totalFreeSpace() const;
        uint16_t getStoreAddress(uint16_t size) const;
        
};
static FATClass FAT;



