#include "EEPROM.h"
#include "instruction_set.h"

#define BUFSIZE 12
static char commandBuffer[BUFSIZE];



// -----------------------------------------------------------------------
#define FILENAMESIZE 12
#define FATSIZE 10
#define FATENTRYSIZE 16
#define FATSTART 1

typedef struct FATEntry{
    char name[FILENAMESIZE];
    uint16_t size;
    uint16_t address;
};

class FATClass{
    private:
        FATEntry FAT[FATSIZE];
    public: 
        FATEntry operator [] (int i) const{
            EEPROM.get(FATSTART + FATENTRYSIZE*i, FAT[i]);
            return FAT[i];
        }
};
static EERef noOfFiles = EEPROM[0];
static FATClass FAT;
// -----------------------------------------------------------------------



void setup(){
    Serial.begin(9600);
}

void loop(){
    executeCommand();
    delayMicroseconds(65500);
}

void doWhileWaiting(){
    delay(1);
}



// -----------------------------------------------------------------------











void store(){
    Serial.print("File name: ");
    while(!readCommand(commandBuffer)) doWhileWaiting();
    Serial.println(commandBuffer);

    Serial.print("File size: ");
    while(!readCommand(commandBuffer)) doWhileWaiting();
    Serial.println(commandBuffer);


    clearBuffer();
}

void retrieve(){
    Serial.println("Retrieve function");
    Serial.println(EEPROM.length());
    uint8_t size = sizeof(FATEntry);
    Serial.println(size);
    // // FATEntry file = {"hoi", 69, 420};
    // // EEPROM.put(1, file);
    Serial.print(FAT[0].name);
    Serial.print(FAT[0].size);
    Serial.println(FAT[0].address);
    clearBuffer();
}











// -----------------------------------------------------------------------
// Command name and reference to the function
typedef struct commandType{
    char name[BUFSIZE];
    void (*func)();
};

static commandType command[] = {
    {"store", &store},
    {"retrieve", &retrieve}
};
static const uint8_t NoOfCommands = sizeof(command) / sizeof(commandType);


bool executeCommand(){

    if(readCommand(commandBuffer)) {

        for(const commandType c : command){
            if(strcmp(c.name, commandBuffer) == 0) {
                commandBuffer[0] = '\0';        // "erases" the buffer
                c.func();
                
                return true; 
            }
        }
        
        Serial.print("Error: no command \"");
        Serial.print(commandBuffer);
        Serial.println('"');

        Serial.println("-------------------------------------------");
        Serial.println("Available commands:");
        for(const commandType c : command){
            Serial.println(c.name);
        }
        Serial.println("-------------------------------------------");


        // Get rid of any arguments appended to the command
        clearBuffer();

        return true; 
        
    }
    
    return false;
}

// Reads a single character from de serial buffer and places it in commandBuffer, returns true if command is complete
bool readCommand(char* buffer){
    char* c = buffer + strlen(buffer);
    
    while(Serial.available()){

        *c = Serial.read();

        // if the buffer is empty and the received character is a space, Line Finished or Cariiage Return, it gets ignored
        if(strlen(buffer) == 0 && (*c == ' ' || *c == '\n' || *c == '\r')) return false;

        // If space, LF or CR, finish command with null terminator
        if(*c == ' ' || *c == '\n' || *c == '\r') {
            *c = '\0';
            return true;
        }
        
        ++c;
        *c = '\0';

        // Max length reached
        if(c == buffer + BUFSIZE - 1) return true;

    }
    return false;
}

void clearBuffer(){
    commandBuffer[0] = '\0';        // "erases" the commandBuffer 

    delay(2);
    while(Serial.available()) {
        Serial.read();
        delay(2);
    }
}