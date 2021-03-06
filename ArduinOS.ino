#include "EEPROM.h"

#include "instruction_set.h"
#include "errorCodes.hpp"
#include "FAT.hpp"


// -----------------------------------------------------------------------

#define COMMANDLENGTH 12
const char SEPARATOR[] = "---------------------------------------------------------";
int16_t usingCommandLine = -1;
int32_t readInt(uint8_t digits = 4);
#define BYTESPERLINE 20

int16_t printErrorCode(int16_t code){
    switch (code){
    case 1:
        // Success!
        return 1;

    case 0:
        Serial.println(F("General exception"));
        break;

    case NOTFOUND:
        Serial.println(F("Not found"));
        break;

    case FORMATERROR:
        Serial.println(F("Incorrect format"));
        break;
    
    case TERMINATED:
        Serial.println(F("Process is terminated"));
        break;

    case MAXACTIVEERROR:
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

    case NOTAVALUE:
        Serial.println(F("Not a value exception"));
        break;
    
    case TYPEERROR:
        Serial.println(F("Type error exception"));
        break;

    case ENDOFEEPROM:
        Serial.println(F("End of EEPROM exception"));
        break;

    case EMPTYARGERROR:
        Serial.println(F("Empty argument exception"));
        break;

    case INSUFFICIENTSPACE:
        Serial.println(F("Insufficient available EEPROM exception"));
        break;

    default:
        Serial.print(F("Unknown exception: "));
        Serial.println(code);
        break;
    }

    // delay(1000);
    return code;
        
}


// -----------------------------------------------------------------------

#include "procTable.hpp"

static uint8_t maxProcesses = 64;
static uint8_t maxActive  = 10;

static process* procTable = new process[maxProcesses];
static uint8_t nActiveProcesses = 0;
static uint8_t nProcesses = 0;
static ProcTableClass ptc(procTable, &nActiveProcesses, &nProcesses, &maxProcesses, &maxActive);




#include "memory.hpp"

static uint16_t memSize = 256;
static uint8_t varTableSize  = 32;

static byte* memory = new byte[memSize];
static variable* varTable = new variable[varTableSize];
static uint8_t nVariables = 0;

static MemoryClass mem(memory, varTable, &nVariables, &memSize, &varTableSize, procTable);




#include "processExecution.hpp"

static processExecution pE(procTable, &mem, &ptc);


// -----------------------------------------------------------------------

void setup(){
    Serial.begin(9600);
}

void loop(){
    if(usingCommandLine == -1) executeCommand();
    else if(procTable[usingCommandLine].status == 't') usingCommandLine = -1;
    else if(Serial.available() && Serial.read() == 'd'){
        ptc.terminateProcess(usingCommandLine);
        Serial.println(F("Terminated"));
        usingCommandLine = -1;
    }

    executeProcesses();
}

void executeProcesses(){
    uint8_t foundActive = 0;

    for(uint8_t i = 0; i < nProcesses; i++){

        if(procTable[i].status != 't'){
            
            if(procTable[i].status == 'r'){
                int16_t exitCode = pE.executeInstruction(i);
                if(exitCode != 1){
                    printErrorCode(exitCode);
                    ptc.terminateProcess(i);
                    // procTable[i].status = 't';
                    Serial.print(F("Process "));
                    Serial.print(i);
                    Serial.print(F(" terminated with exit code: "));
                    Serial.println(exitCode);
                };
            }
                
            foundActive++;
            if(foundActive >= nActiveProcesses) break;
        }
    }
}



// -----------------------------------------------------------------------


void test(){
    FREERAM_PRINT;

    // for(int i = 0; i < nProcesses; i++){
    //     Serial.print((uint16_t) mem.getVar('p', i));
    //     Serial.print(' ');
    //     Serial.print((uint16_t) procTable[i].data->pc);
    //     Serial.print(' ');
    //     Serial.print((uint16_t) procTable[i].data);
    //     Serial.print(' ');
    //     Serial.print((uint16_t) stack(i).stack);
    //     Serial.print(' ');
    //     Serial.println(stack(i).stackPtr);
    // }
    
    // Serial.println(stack(1).stackPtr);

    // Serial.println((char) stack(0).popChar());
    // Serial.println(stack(0).popInt());

    // FILECOUNT = 6;

    // char s[stack(0).peekByte(1)];
    // stack(0).popString(s);
    // Serial.println(s);


    
    
    // Serial.println((int) sizeof(ptc));
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
    // stack(0).pushString(string);

    // Serial.println((char) stack(11).popChar());

    // Serial.println(stack(11).popFloat());

    // Serial.println(stack(11).popInt());

    // Serial.println(stack(11).popInt());

    // char s[stack(0).peekByte(1)];
    // stack(0).popString(s);
    // Serial.println(s);


    // Serial.println(mem.freeSpace(4));
    // // Serial.println(mem.freeSpace(250));
    // printErrorCode(mem.freeSpace(250));

    // ptc.terminateProcess(11);

    // stack(11).pushInt(69);
    // stack(11).pushInt(420);

    // printErrorCode(mem.set('a', 11)); // 420
    // printErrorCode(mem.set('b', 11)); // 69

    // printErrorCode(mem.get('a', 11));
    // printErrorCode(mem.get('b', 11));

    // Serial.println(stack(11).popInt());
    // Serial.println(stack(11).popInt());


    // stack(10).pushFloat(1.1);
    // stack(10).pushChar('c');

    // printErrorCode(mem.set('a', 10, &procTable[10]));
    // printErrorCode(mem.set('b', 10, &procTable[10]));

    // printErrorCode(mem.get('a', 10, &procTable[10]));
    // printErrorCode(mem.get('b', 10, &procTable[10]));

    // Serial.println(stack(10).popFloat());
    // Serial.println(stack(10).popChar());

    
    // for(int i = 0; i < 12; i++){
    //     printErrorCode(pE.executeInstruction(0));
    // }

    // Serial.println(stack(0).popInt());

    
}

// -----------------------------------------------------------------------


void run(){
    Serial.print(F("Program name: "));
    char programName[FILENAMELENGTH] = ""; 
    readCommand(programName, FILENAMELENGTH);
    Serial.println(programName);

    if(printErrorCode(ptc.addProcess(programName)) != 1) return;


    Serial.print(F("Foreground process started with id: "));
    Serial.println(nProcesses - 1);
    Serial.println(SEPARATOR);
    usingCommandLine = nProcesses -1;
}

void runBG(){
    Serial.print(F("Program name: "));
    char programName[FILENAMELENGTH] = ""; 
    readCommand(programName, FILENAMELENGTH);
    Serial.println(programName);

    int16_t id = ptc.addProcess(programName);
    if(id < 0){
        printErrorCode(id);
        return;
    }


    Serial.print(F("Background process started with id: "));
    Serial.println(id);
}

void list(){
    Serial.println(F("ID \t\t\tName\t\t\tStatus"));

    for(int i = 0; i < nProcesses; i++){
        char name[FILENAMELENGTH];
        ptc.getProcessName(i, name);

        char format[27];
        sprintf(format, "%-3i %12s\t\t\t  %c", i, name, procTable[i].status);
        Serial.println(format);
    }
}


uint8_t readId(){
    Serial.print(F("Process id: "));
    const uint8_t id = readInt(3);
    Serial.println(id);
    return id;
}

void suspend(){
    const uint8_t id = readId();

    if(printErrorCode(ptc.suspendProcess(id)) != 1) return;

    Serial.println(F("Process suspended"));
}

void resume(){
    const uint8_t id = readId();

    if(printErrorCode(ptc.resumeProcess(id)) != 1) return;

    Serial.println(F("Process resumed"));
}

void terminate(){
    const uint8_t id = readId();

    if(printErrorCode(ptc.terminateProcess(id)) != 1) return;

    mem.eraseAll(id);
    Serial.println(F("Process terminated"));

}

void editVar(){
    const uint8_t id = readId();

    Serial.print(F("Variable name: "));

    uint8_t name = readByte();
    if(name == 0) return;
    Serial.println((char) name);

    if(mem.getVar(name, id) == nullptr){
        printErrorCode(NOTFOUND);
        return;
    }

    Serial.print(F("Numeric type: "));
    char type[6] = ""; 
    readCommand(type, 6);
    Serial.println(type);


    Serial.print(F("Value: "));
    
    if(strcmp(type, "char") == 0){
        byte c = readByte();
        Serial.println(c);
        printErrorCode(stack(id).pushChar(c));
    }
    else if(strcmp(type, "int") == 0){
        int16_t i = readInt(6);
        Serial.println(i);
        printErrorCode(stack(id).pushInt(i));
    }
    else if(strcmp(type, "float") == 0){
        char value[12];
        readCommand(value, 12);
        float f = atof(value);
        Serial.println(f);
        printErrorCode(stack(id).pushFloat(f));
    }
    else{
        printErrorCode(TYPEERROR);
        return;
    }

    printErrorCode(mem.set(name, id));
}

void setPC(){
    const uint8_t id = readId();

    Serial.print(F("Program counter address: "));
    char addressChar[FILESIZEDIGITS] = "";
    const uint16_t address = readInt();
    Serial.println(address);

    procTable[id].data->pc = address;
}

void readStack(){
    const uint8_t id = readId();

    Serial.print(F("Stackpointer: "));
    Serial.println(stack(id).stackPtr);

    for(uint16_t line = 0; line < STACKSIZE; line += BYTESPERLINE){
        for(uint16_t i = line; i < line + BYTESPERLINE && i < STACKSIZE; i++){
            char value[7];
            sprintf(value, "%4i| ", i);
            Serial.print(value);
        }
        Serial.println();
        for(uint16_t i = line; i < line + BYTESPERLINE && i < STACKSIZE; i++){
            char value[7];
            sprintf(value, "%4i| ", (uint8_t) *(stack(id).stack+i));
            Serial.print(value);
        }
        Serial.println('\n');
    }
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
    const uint16_t fileSize = readInt();
    Serial.println(fileSize);

    const uint16_t free = FAT.largestFreeSpace();
    if(free < fileSize){
        Serial.println(F("Insufficient space"));
        freeSpace();
        return;
    }
    else if(fileSize == 0){
        Serial.println(F("Filesize 0 not allowed"));
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

        executeProcesses();
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
        printErrorCode(NOTFOUND);
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
    printErrorCode(NOTFOUND);
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
    Serial.print(F("Total free space: \t\t"));
    Serial.print(buf);
    Serial.println(F(" bytes"));
}

void readEEPROM(){
    Serial.print(F("Address: "));
    const uint16_t address = readInt();
    Serial.println(address);

    Serial.print(F("Number of bytes: "));
    const uint16_t nBytes = readInt();
    Serial.println(nBytes);


    for(uint16_t line = 0; line < nBytes; line += BYTESPERLINE){
        for(uint16_t i = line; i < line + BYTESPERLINE && i < nBytes; i++){
            char value[6];
            sprintf(value, "%3i| ", address+i);
            Serial.print(value);
        }
        Serial.println();
        for(uint16_t i = line; i < line + BYTESPERLINE && i < nBytes; i++){
            char value[6];
            sprintf(value, "%3i| ", (uint8_t) EEPROM[address+i]);
            Serial.print(value);
        }
        Serial.println('\n');
    }
    

}

void writeEEPROM(){
    Serial.print(F("Address: "));
    char addressChar[FILESIZEDIGITS] = "";
    const uint16_t address = readInt();
    Serial.println(address);

    Serial.print(F("Number of bytes: "));
    const uint16_t nBytes = readInt();
    Serial.println(nBytes);

    Serial.println(F("Bytes:"));
    
    for(uint16_t i = 0; i < nBytes; i++){
        Serial.print(address+i);
        Serial.print(F(" was: "));
        Serial.print((uint8_t) EEPROM[address+i]);
        Serial.print(F("  is now: "));

        uint8_t b = readByte();

        EEPROM[address+i] = b;
        Serial.println((uint8_t) EEPROM[address+i]);
    }
    
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
    {"runbg", &runBG},
    {"list", &list},
    {"suspend", &suspend},
    {"resume", &resume},
    {"terminate", &terminate},
    {"editvar", &editVar},
    {"setpc", &setPC},
    {"readstack", &readStack},
    {"store", &store},
    {"retrieve", &retrieve},
    {"erase", &erase},
    {"files", &files},
    {"freespace", &freeSpace},
    {"readeeprom", &readEEPROM},
    {"writeeeprom", &writeEEPROM}
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
    char* c = command;

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

        executeProcesses();
    }
}

byte readByte(){
    char byteChar[9] = "";
    readCommand(byteChar, 9);

    uint8_t b = 0;

    if(strlen(byteChar) == 1 && (byteChar[0] < '0' || byteChar[0] > '9')){
        b = (uint8_t) byteChar[0];
    }

    else if(strlen(byteChar) <= 3) b = (uint8_t) atoi(byteChar);

    else if(strlen(byteChar) == 8){
        char* c = byteChar;
        for(int8_t j = 7; j >= 0; j--){
                b += (*c - '0') << j;
                c++;
        }
    }

    else {
        printErrorCode(FORMATERROR);
        return;
    }

    return b;
}

int32_t readInt(uint8_t digits = 4){
    char intChar[digits+1] = ""; 
    readCommand(intChar, digits+1);
    return atoi(intChar);
}

void clearBuffer(){

    unsigned long time = millis();

    while(millis() - time < 2){
        executeProcesses();
    }

    while(Serial.available()) {
        Serial.read();

        time = millis();
        while(millis() - time < 2){
            executeProcesses();
        }
    }
}