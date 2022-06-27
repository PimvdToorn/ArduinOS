#include "Arduino.h"
#include "EEPROM.h"

#define EEPROMSIZE 1024

#define FATSIZE 10
#define FATENTRYSIZE 16
#define FATSTART 1

#define FILENAMELENGTH 12
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

        void addEntry(const char* name, const uint16_t* size, const uint16_t* address); 
        bool deleteEntry(char* name);

        FATEntry findFile(char* name);

        uint16_t largestFreeSpace();
        uint16_t totalFreeSpace();
        uint16_t getStoreAddress(uint16_t size);
        
};



