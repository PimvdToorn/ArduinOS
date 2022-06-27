#include "EEPROM.h"

#include "instruction_set.h"
#include "errorCodes.hpp"
#include "FAT.hpp"


// -----------------------------------------------------------------------

#define COMMANDLENGTH 12
const char SEPARATOR[] = "---------------------------------------------------------";


int16_t checkErrorCode(int16_t code){
    switch (code){
    case 1:
        // Success!
        break;

    case 0:
        Serial.println(F("General exception"));
        break;

    case NOTFOUND:
        Serial.println(F("Not found"));
        break;
    
    case TERMINATED:
        Serial.println(F("Process is terminated"));
        break;

    case maxActiveERROR:
        Serial.println(F("Max active processes reached"));
        break;

    case MAXTOTALERROR:
        Serial.println(F("Max total processes reached"));
        break;

    case STACKFULLERROR:
        Serial.println(F("Stack full exception"));
        break;

    case MEMFULLERROR:
        Serial.println(F("Memory full exception"));
        break;

    case VARTABLEFULLERROR:
        Serial.println(F("Variable table full exception"));
        break;

    case UNKNOWNINSTRUCTION:
        Serial.println(F("Unknown instruction exception"));
        break;

    default:
        Serial.print(F("Unknown exception: "));
        Serial.println(code);
        break;
    }

    return code;
        
}


// -----------------------------------------------------------------------

#include "procTable.hpp"

static uint8_t maxProcesses = 64;
static uint8_t maxActive  = 10;

static process* procTable = new process[maxProcesses];
static uint8_t nActiveProcesses = 0;
static uint8_t nProcesses = 0;
static ProcTableClass pT(procTable, &nActiveProcesses, &nProcesses, &maxProcesses, &maxActive);




#include "memory.hpp"

static uint16_t memSize = 256;
static uint8_t varTableSize  = 32;

static byte* memory = new byte[memSize];
static variable* varTable = new variable[varTableSize];
static uint8_t nVariables = 0;

static MemoryClass mem(memory, varTable, &nVariables, &memSize, &varTableSize, procTable);


#include "processExecution.hpp"

static processExecution pE(procTable, &mem);


// -----------------------------------------------------------------------

void setup(){
    Serial.begin(9600);

    checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));
    // checkErrorCode(pT.addProcess("blink"));

    // for(int i = 0; i < 2; i++){
    //     checkErrorCode(pT.terminateProcess(i));
    //     checkErrorCode(pT.addProcess("blink"));
    // }


}

void loop(){
    executeCommand();
    delayMicroseconds(65500);
}

void doWhileWaiting(){
    delay(1);
}



// -----------------------------------------------------------------------


void test(){
    FREERAM_PRINT;

    
    
    // Serial.println((int) sizeof(pT));
    // Serial.println((int) sizeof(procTable));
    // Serial.println((int) sizeof(procTable[6]));
    // Serial.println((int) sizeof(*procTable[6].data));


    // Serial.println((int) sizeof(mem));

    // Serial.println((int) sizeof(varTable));
    // Serial.println((int) sizeof(varTable[0]));

    // Serial.println((int) sizeof(memory));
    // Serial.println((int) sizeof(memory[0]));

    // stack(11).pushInt(69);
    // stack(11).pushInt(420);
    // stack(10).pushFloat(1.1);
    // stack(10).pushChar('c');

    // char string[] = "Hallo";
    // stack(10).pushString(string);

    // Serial.println((char) stack(11).popChar());

    // Serial.println(stack(11).popFloat());

    // Serial.println(stack(11).popInt());

    // Serial.println(stack(11).popInt());

    // char s[stack(10).peekStrLen()];
    // stack(10).popString(s);
    // Serial.println(s);


    // Serial.println(mem.freeSpace(4));
    // // Serial.println(mem.freeSpace(250));
    // checkErrorCode(mem.freeSpace(250));

    // pT.terminateProcess(11);

    // stack(11).pushInt(69);
    // stack(11).pushInt(420);

    // checkErrorCode(mem.set('a', 11)); // 420
    // checkErrorCode(mem.set('b', 11)); // 69

    // checkErrorCode(mem.get('a', 11));
    // checkErrorCode(mem.get('b', 11));

    // Serial.println(stack(11).popInt());
    // Serial.println(stack(11).popInt());


    // stack(10).pushFloat(1.1);
    // stack(10).pushChar('c');

    // checkErrorCode(mem.set('a', 10, &procTable[10]));
    // checkErrorCode(mem.set('b', 10, &procTable[10]));

    // checkErrorCode(mem.get('a', 10, &procTable[10]));
    // checkErrorCode(mem.get('b', 10, &procTable[10]));

    // Serial.println(stack(10).popFloat());
    // Serial.println(stack(10).popChar());

    
    for(int i = 0; i < 12; i++){
        checkErrorCode(pE.executeInstruction(0));
    }

    Serial.println(stack(0).popInt());

    
}

// -----------------------------------------------------------------------


void run(){
    Serial.print(F("Program name: "));
    char programName[FILENAMELENGTH] = ""; 
    readCommand(programName, FILENAMELENGTH);
    Serial.println(programName);

    if(!checkErrorCode(pT.addProcess(programName))) return;


    Serial.print(F("Process started with id: "));
    Serial.println(nProcesses - 1);
}

void list(){
    Serial.println(F("ID \t\t\tName\t\tStatus"));

    for(int i = 0; i < nProcesses; i++){
        char name[FILENAMELENGTH];
        pT.getProcessName(i, name);

        char format[27];
        sprintf(format, "%-3i %12s\t\t\t  %c", i, name, procTable[i].status);
        Serial.println(format);
    }
}



void suspend(){
    Serial.print(F("Process id: "));
    char idChar[4] = "";
    readCommand(idChar, 4);
    const uint8_t id = atoi(idChar);
    Serial.println(id);

    if(!checkErrorCode(pT.suspendProcess(id))) return;

    Serial.println(F("Process suspended"));
}

void resume(){
    Serial.print(F("Process id: "));
    char idChar[4] = "";
    readCommand(idChar, 4);
    const uint8_t id = atoi(idChar);
    Serial.println(id);

    if(!checkErrorCode(pT.resumeProcess(id))) return;

    Serial.println(F("Process resumed"));
}

void terminate(){
    Serial.print(F("Process id: "));
    char idChar[4] = "";
    readCommand(idChar, 4);
    const uint8_t id = atoi(idChar);
    Serial.println(id);

    if(!checkErrorCode(pT.terminateProcess(id))) return;

    mem.eraseAll(id);
    Serial.println(F("Process terminated"));

}





// -----------------------------------------------------------------------


void store(){
    if(FILECOUNT >= FATSIZE){
        Serial.println(F("FAT limit reached"));
        return;
    }

    Serial.print(F("File name: "));
    char fileName[FILENAMELENGTH] = ""; 
    readCommand(fileName, FILENAMELENGTH);
    Serial.println(fileName);

    if(strcmp(fileName, FAT.getFATEntry(fileName).name) == 0){
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

        freeSpace();

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

    FATEntry file = FAT.getFATEntry(fileName);

    if(file.size == 0) {
        checkErrorCode(NOTFOUND);
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
    checkErrorCode(NOTFOUND);
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
    char buf[5];

    sprintf(buf, "%4i", FAT.largestFreeSpace());
    Serial.print(F("Largest free block: \t"));
    Serial.print(buf);
    Serial.println(F(" bytes"));


    sprintf(buf, "%4i", FAT.totalFreeSpace());
    Serial.print(F("Total free space:\t\t"));
    Serial.print(buf);
    Serial.println(F(" bytes"));
}



// -----------------------------------------------------------------------
// Command name and reference to the function
typedef struct commandType{
    char name[COMMANDLENGTH];
    void (*func)();
};

static commandType commands[] = {
    {"test", &test},
    {"run", &run},
    {"list", &list},
    {"suspend", &suspend},
    {"resume", &resume},
    {"terminate", &terminate},
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