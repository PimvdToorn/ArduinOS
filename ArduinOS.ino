#include "instruction_set.h"
#include "FAT.hpp"
#include "EEPROM.h"

#define COMMANDLENGTH 12


const char SEPARATOR[] = "---------------------------------------------------------";


// "output": "C:\\Users\\Pim\\Documents\\.Me\\IT\\Arduino\\VSCode output",

// -----------------------------------------------------------------------

#define FILESIZEDIGITS 5

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







// 4062 430
// 4062 412
// 4398 408
// 4408 408
// 4490 318
// 4616 318


void store(){
    Serial.print(F("File name: "));
    char fileName[FILENAMELENGTH] = ""; 
    readCommand(fileName, FILENAMELENGTH);
    Serial.println(fileName);

    if(strcmp(fileName, FAT.findFile(fileName).name) == 0){
        Serial.println(F("Filename is taken"));

        return;
    }
    

    Serial.print(F("File size: "));
    char fileSizeChar[FILESIZEDIGITS] = "";
    readCommand(fileSizeChar, FILESIZEDIGITS);
    const uint16_t fileSize = atoi(fileSizeChar);
    Serial.println(fileSize);

    const uint16_t free = FAT.largestFreeSpace();
    if(free < fileSize){
        Serial.println(F("Insufficient space"));

        Serial.print(F("Maximum available file size is "));
        Serial.print(free);
        Serial.println(F(" bytes"));

        Serial.print(F("Total available space is "));
        Serial.print(FAT.totalFreeSpace());
        Serial.println(F(" bytes.   (Requires defragmentation)"));

        return;
    }

    const uint16_t address = FAT.getStoreAddress(fileSize);
    
    Serial.print(F("address: ")); 
    Serial.println(address); 

    Serial.println(F("Content:"));

    uint16_t bytesWritten = 0;
    while(bytesWritten<fileSize){
        if(Serial.available()){
            const char c = Serial.read();
            EEPROM.update(address + bytesWritten, c);
            ++bytesWritten;

            Serial.print(c);
        }

        doWhileWaiting();
    }
    Serial.println();
    Serial.println(F("Done writing"));

    FAT.addEntry(fileName, &fileSize, &address);

    Serial.print(F("Number of files: "));
    Serial.println(FILECOUNT);
}

void retrieve(){
    Serial.println(SEPARATOR);


    Serial.print(F("Retrieve file: "));
    char fileName[FILENAMELENGTH] = ""; 
    readCommand(fileName, FILENAMELENGTH);
    Serial.println(fileName);

    FATEntry file = FAT.findFile(fileName);

    if(file.size == 0) {
        Serial.print(F("File \""));
        Serial.print(fileName);
        Serial.println(F("\" does not exist"));
        return;
    } 

    Serial.println(F("Content:"));
    Serial.println(SEPARATOR);

    uint16_t bytesPrinted = 0;
    while(bytesPrinted < file.size){
        Serial.print((char) EEPROM[file.address + bytesPrinted]);

        ++bytesPrinted;
    }
    Serial.println();
}

void erase(){
    Serial.print(F("Delete file: "));
    char fileName[FILENAMELENGTH] = ""; 
    readCommand(fileName, FILENAMELENGTH);
    Serial.println(fileName);

    if(FAT.deleteEntry(fileName)){
        Serial.println(F("Deletion succesful"));
        return;
    }
    Serial.println(F("File not found"));
}

void files(){
    Serial.println(F("File name   \tSize\t\tAddress"));

    for(int i = 0; i < FILECOUNT; i++){
        char format[27];
        sprintf(format, "%-12s\t%4u\t\t   %4u", FAT[i].name, FAT[i].size, FAT[i].address);
        Serial.println(format);
    }
}

void freeSpace(){
    Serial.print(F("Largest free block:\t\t"));
    Serial.print(FAT.largestFreeSpace());
    Serial.println(F(" bytes"));
    Serial.print(F("Total free space:\t\t"));
    Serial.print(FAT.totalFreeSpace());
    Serial.println(F(" bytes"));
}



// -----------------------------------------------------------------------
// Command name and reference to the function
typedef struct commandType{
    char name[COMMANDLENGTH];
    void (*func)();
};

static commandType commands[] = {
    {"store", &store},
    {"retrieve", &retrieve},
    {"erase", &erase},
    {"files", &files},
    {"freespace", &freeSpace}
};
static const uint8_t NoOfCommands = sizeof(commands) / sizeof(commandType);


void executeCommand(){
    clearBuffer();
    Serial.println(SEPARATOR);
    Serial.print("Execute command: ");


    char command[COMMANDLENGTH] = "";
    readCommand(command, COMMANDLENGTH);
 
    Serial.println(command);

    for(const commandType c : commands){
        if(strcmp(c.name, command) == 0) {
            c.func();
            return;
        }
    }
    
    Serial.print("Error: no command \"");
    Serial.print(command);
    Serial.println('"');

    Serial.println(SEPARATOR);
    Serial.println("Available commands:");
    for(const commandType c : commands){
        Serial.println(c.name);
    }

    // Get rid of any arguments appended to the command
    clearBuffer();        
}

// Reads a single command from the command line and returns it, 
void readCommand(char* command, const int length){
    char* c = command + strlen(command);

    while(true){
        if(Serial.available()){

            *c = Serial.read();

            // if the buffer is empty and the received character is a space, Line Finished or Cariiage Return, it gets ignored
            if(strlen(command)  <= 1 && (*c == ' ' || *c == '\n' || *c == '\r')) *c = '\0';

            // If space, LF or CR, finish command
            else if(*c == ' ' || *c == '\n' || *c == '\r'){
                *c = '\0';
                return;
            }
            
            else{
                ++c;
                *c = '\0';
            }

            if(c == command + length - 1) return;
        }

        doWhileWaiting();
    }
}

void clearBuffer(){

    delay(2);
    while(Serial.available()) {
        Serial.read();
        delay(2);
    }
}