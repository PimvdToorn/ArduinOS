#include "instruction_set.h"

#define BUFSIZE 12

#define STOPPED 0
#define RUNNING 1
// #define BUSY 2
// #define FINISHED 3
// #define WAITING 4


typedef struct process{
    // uint8_t id;
    String name;
    uint8_t status;
    // uint8_t waitForID;
};

process processes[] = {
    {"CommandLine", STOPPED},
    {"Run", RUNNING}
};


// -----------------------------------------------------------------------
static char commandBuffer[BUFSIZE];

typedef struct commandType{
    char name[BUFSIZE];
    void (*func)();
};

void store();
void retrieve();

// Command name and reference to the function
static commandType command[] = {
    {"store", &store},
    {"retrieve", &retrieve}
};
static const uint8_t NoOfCommands = sizeof(command) / sizeof(commandType);
//------------------------------------------------------------------------



void setup(){
    Serial.begin(9600);
    
}

void loop(){
    // for(const process& p : processes){
    //     if(p.status == RUNNING)
    // }
    run();
    delayMicroseconds(100);
}


void store(){
    // if(readCommand(commandBuffer)){
    //     Serial.println("Store function");
    //     Serial.println(commandBuffer);
    // }
    Serial.println("Store");
}

void retrieve(){
    Serial.println("Retrieve function");
}



bool run(){

    if(readCommand(commandBuffer)) {

        // for(const commandType& c : command){
        //     if(strcmp(c.name, commandBuffer) == 0) {
        //         commandBuffer[0] = '\0';        // "erases" the buffer
        //         c.func();
                
        //         return true; 
        //     }
        // }
        for(int i = 0; i < NoOfCommands; i++){
            if(strcmp(command[i].name, commandBuffer) == 0) {
                commandBuffer[0] = '\0';        // "erases" the buffer
                command[i].func();
                
                return true; 
            }
        }
        
        Serial.print("Error: no command \"");
        Serial.print(commandBuffer);
        Serial.println('"');
        commandBuffer[0] = '\0';        // "erases" the buffer 

        // Clears the serial buffer to get rid of any arguments appended to the command
        delay(2);
        while(Serial.available() > 0) {
            Serial.read();
            delay(2);
        }

        return true; 
        
    }
    
    return false;
}

bool readCommand(char* buffer){

    // 32 space
    // 10 Line Finished
    
    if(Serial.available()){
        const char receivedCharacter = Serial.read();
        
        // if the buffer is empty and the received character is a space or LF, it gets ignored
        if(strlen(buffer) == 0 && (receivedCharacter == 32 || receivedCharacter == 10)) return false;

        if(receivedCharacter == 32 || receivedCharacter == 10) {
            strcat(buffer, '\0');
            return true;
        }
        else{
            strcat(buffer, &receivedCharacter);
        }  

        // if the buffer lenght is 11 or higher, the 12th char will be made the terminating character
        // and the serial buffer will be looped through till the first space or LF
        if(strlen(buffer) >= BUFSIZE-1){
            buffer[BUFSIZE-1] = '\0';

            char nextCharacter = '\0';
            while(nextCharacter != 32 && nextCharacter != 10) nextCharacter = Serial.read();

            return true;
        }   
    }
    return false;
}