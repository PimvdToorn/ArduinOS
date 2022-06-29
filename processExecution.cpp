#include "processExecution.hpp"

processExecution::processExecution(process* procTable, MemoryClass* memory, ProcTableClass* ptc):
    procTable (procTable), memory (memory), ptc (ptc){}





int16_t processExecution::unaryOperators(const uint8_t id, const byte uOperator){
    const byte type = pStack.peekByte();
    if(!(type < 5 && type != 3)) return NOTAVALUE;

    // const float number = pStack.popVal();

    switch (uOperator){
    case INCREMENT:
        switch (type){
        case CHAR:
            return pStack.pushChar(pStack.popChar() + 1);
        case INT:
            return pStack.pushInt(pStack.popInt() + 1);
        case FLOAT:
            return pStack.pushFloat(pStack.popFloat() + 1);
        }

    case DECREMENT:
        switch (type){
        case CHAR:
            return pStack.pushChar(pStack.popChar() - 1);
        case INT:
            return pStack.pushInt(pStack.popInt() - 1);
        case FLOAT:
            return pStack.pushFloat(pStack.popFloat() - 1);
        }
    
    // case UNARYMINUS:

    // case LOGICALNOT:

    // case BITWISENOT:

    // case TOCHAR:

    // case TOINT:

    // case TOFLOAT:

    // case ROUND:

    // case FLOOR:

    // case CEIL:

    // case ABS:

    // case SQ:

    // case SQRT:

    // case ANALOGREAD:

    // case DIGITALREAD:

    }

}

int16_t processExecution::binaryOperators(const uint8_t id, const byte bOperator){
    const byte type2 = pStack.peekByte();

    char c2;
    int16_t i2;
    float f2;

    if(type2 == CHAR) c2 = pStack.popChar();
    else if(type2 == INT) i2 = pStack.popInt();
    else if(type2 == FLOAT) f2 = pStack.popFloat();



    const byte type1 = pStack.peekByte();

    const byte type = max(type1,type2);
    if(type > 5 or type == 3) return NOTAVALUE;

    char c1;
    int16_t i1;
    float f1;

    if(type == CHAR){
        c1 = pStack.popChar();
    }
    else if(type == INT){
        if(type2 == CHAR) i2 = (int16_t) c2;

        if(type1 == INT) i1 = pStack.popInt();
        else i1 = pStack.popChar();
    }
    else{
        if(type2 == CHAR) f2 = (float) c2;
        else if(type2 == INT) f2 = (float) i2;

        f1 = pStack.popVal();
    }


    switch (bOperator){
    case PLUS:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 + c2);
        case INT:
            return pStack.pushInt(i1 + i2);
        case FLOAT:
            return pStack.pushFloat(f1 + f2);
        }

    case MINUS:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 - c2);
        case INT:
            return pStack.pushInt(i1 - i2);
        case FLOAT:
            return pStack.pushFloat(f1 - f2);
        }

    case TIMES:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 * c2);
        case INT:
            return pStack.pushInt(i1 * i2);
        case FLOAT:
            return pStack.pushFloat(f1 * f2);
        }

    case DIVIDEDBY:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 / c2);
        case INT:
            return pStack.pushInt(i1 / i2);
        case FLOAT:
            return pStack.pushFloat(f1 / f2);
        }

    case MODULUS:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 % c2);
        case INT:
            return pStack.pushInt(i1 % i2);
        case FLOAT:
            return TYPEERROR;
        }
    
    case EQUALS:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 == c2);
        case INT:
            return pStack.pushChar(i1 == i2);
        case FLOAT:
            return pStack.pushChar(f1 == f2);
        }

    case NOTEQUALS:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 != c2);
        case INT:
            return pStack.pushChar(i1 != i2);
        case FLOAT:
            return pStack.pushChar(f1 != f2);
        }

    case LESSTHAN:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 < c2);
        case INT:
            return pStack.pushChar(i1 < i2);
        case FLOAT:
            return pStack.pushChar(f1 < f2);
        }

    case LESSTHANOREQUALS:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 <= c2);
        case INT:
            return pStack.pushChar(i1 <= i2);
        case FLOAT:
            return pStack.pushChar(f1 <= f2);
        }

    case GREATERTHAN:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 > c2);
        case INT:
            return pStack.pushChar(i1 > i2);
        case FLOAT:
            return pStack.pushChar(f1 > f2);
        }

    case GREATERTHANOREQUALS:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 >= c2);
        case INT:
            return pStack.pushChar(i1 >= i2);
        case FLOAT:
            return pStack.pushChar(f1 >= f2);
        }

    case LOGICALAND:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 && c2);
        case INT:
            return pStack.pushChar(i1 && i2);
        case FLOAT:
            return pStack.pushChar(f1 && f2);
        }

    case LOGICALOR:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 || c2);
        case INT:
            return pStack.pushChar(i1 || i2);
        case FLOAT:
            return pStack.pushChar(f1 || f2);
        }

    case LOGICALXOR:
        switch (type){
        case CHAR:
            return pStack.pushChar((c1 == false) ^ (c2 == false));
        case INT:
            return pStack.pushChar((i1 == false) ^ (i2 == false));
        case FLOAT:
            return pStack.pushChar((f1 == false) ^ (f2 == false));
        }

    case BITWISEAND:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 & c2);
        case INT:
            return pStack.pushInt(i1 & i2);
        case FLOAT:
            return TYPEERROR;
        }

    case BITWISEOR:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 | c2);
        case INT:
            return pStack.pushInt(i1 | i2);
        case FLOAT:
            return TYPEERROR;
        }

    case BITWISEXOR:
        switch (type){
        case CHAR:
            return pStack.pushChar(c1 ^ c2);
        case INT:
            return pStack.pushInt(i1 ^ i2);
        case FLOAT:
            return TYPEERROR;
        }

    case MIN:
        switch (type){
        case CHAR:
            return pStack.pushChar(min(c1, c2));
        case INT:
            return pStack.pushInt(min(i1, i2));
        case FLOAT:
            return pStack.pushFloat(min(f1, f2));
        }

    case MAX:
        switch (type){
        case CHAR:
            return pStack.pushChar(max(c1, c2));
        case INT:
            return pStack.pushInt(max(i1, i2));
        case FLOAT:
            return pStack.pushFloat(max(f1, f2));
        }

    case POW:
        switch (type){
        case CHAR:
            return pStack.pushChar(pow(c1, c2));
        case INT:
            return pStack.pushInt(pow(i1, i2));
        case FLOAT:
            return pStack.pushFloat(pow(f1, f2));
        }

    }
}

int16_t processExecution::print(const uint8_t id){
    switch (pStack.peekByte()){
        case CHAR:
            Serial.print(pStack.popChar());
            return 1;

        case INT:
            Serial.print(pStack.popInt());
            return 1;

        case STRING:{
            uint8_t len = pStack.peekByte(1);
            char s[len];
            pStack.popString(s);
            Serial.print(s);
            return 1;
        }
        case FLOAT:
            Serial.print(pStack.popFloat(), 14);
            return 1;

        default:
            return TYPEERROR;
    }
}

int16_t processExecution::write(const uint8_t id){
    uint8_t type = pStack.popByte();
    if(type > 4) return TYPEERROR;

    if(type == STRING){
        uint8_t len = pStack.popByte();

        FP += len; 
        if(FP > EEPROMSIZE) return ENDOFEEPROM;

        for(uint8_t i = 1; i <= len; i++){
            EEPROM[FP-i] = pStack.popByte();
        }
    }
    else{
        FP += type;
        if(FP > EEPROMSIZE) return ENDOFEEPROM;

        for(uint8_t i = 1; i <= type; i++){
            EEPROM[FP-i] = pStack.popByte();
        }
    }

    return 1;
}



int16_t processExecution::executeInstruction(const uint8_t id){
    const uint8_t instruction = pcREAD;
    // Serial.print("Instruction: ");
    // Serial.println(instruction);

    switch (instruction){
    case CHAR:
        pStack.pushByte(pcREAD);
        return pStack.pushByte(CHAR);

    case INT:
        pStack.pushByte(pcREAD);
        pStack.pushByte(pcREAD);
        return pStack.pushByte(INT);

    case STRING:{
        const uint16_t start = PC;

        do{
            pStack.pushByte(pcREAD);
        } while(pStack.peekByte() != '\0');

        pStack.pushByte(PC - start);
        return pStack.pushByte(STRING);
    }

    case FLOAT:
        pStack.pushByte(pcREAD);
        pStack.pushByte(pcREAD);
        pStack.pushByte(pcREAD);
        pStack.pushByte(pcREAD);
        return pStack.pushByte(FLOAT);

    case SET:
        return memory->set(pcREAD, id);

    case GET:
        return memory->get(pcREAD, id);

    case INCREMENT:
        switch (pStack.peekByte()){
            case CHAR:
                return pStack.pushChar(pStack.popChar() + 1);
            case INT:
                return pStack.pushInt(pStack.popInt() + 1);
            case FLOAT:
                return pStack.pushFloat(pStack.popFloat() + 1);
            default:
                return NOTAVALUE;
        }
    
    case DECREMENT:
        switch (pStack.peekByte()){
            case CHAR:
                return pStack.pushChar(pStack.popChar() - 1);
            case INT:
                return pStack.pushInt(pStack.popInt() - 1);
            case FLOAT:
                return pStack.pushFloat(pStack.popFloat() - 1);
            default:
                return NOTAVALUE;
        }

    case PLUS:
        return binaryOperators(id, instruction);

    case MINUS:
        return binaryOperators(id, instruction);

    case TIMES:
        return binaryOperators(id, instruction);

    case DIVIDEDBY:
        return binaryOperators(id, instruction);

    case MODULUS:
        return binaryOperators(id, instruction);

    case UNARYMINUS:
        switch (pStack.peekByte()){
            case CHAR:
                return pStack.pushChar(-pStack.popChar());
            case INT:
                return pStack.pushInt(-pStack.popInt());
            case FLOAT:
                return pStack.pushFloat(-pStack.popFloat());
            default:
                return NOTAVALUE;
        }
    
    case EQUALS:
        return binaryOperators(id, instruction);

    case NOTEQUALS:
        return binaryOperators(id, instruction);

    case LESSTHAN:
        return binaryOperators(id, instruction);

    case LESSTHANOREQUALS:
        return binaryOperators(id, instruction);

    case GREATERTHAN:
        return binaryOperators(id, instruction);

    case GREATERTHANOREQUALS:
        return binaryOperators(id, instruction);

    case LOGICALAND:
        return binaryOperators(id, instruction);

    case LOGICALOR:
        return binaryOperators(id, instruction);

    case LOGICALXOR:
        return binaryOperators(id, instruction);

    case LOGICALNOT:
        switch (pStack.peekByte()){
            case CHAR:
                return pStack.pushChar(!pStack.popChar());
            case INT:
                return pStack.pushChar(!pStack.popInt());
            case FLOAT:
                return pStack.pushChar(!pStack.popFloat());
            default:
                return NOTAVALUE;
        }

    case BITWISEAND:
        return binaryOperators(id, instruction);

    case BITWISEOR:
        return binaryOperators(id, instruction);

    case BITWISEXOR:
        return binaryOperators(id, instruction);

    case BITWISENOT:
        switch (pStack.peekByte()){
            case CHAR:
                return pStack.pushChar(~pStack.popChar());
            case INT:
                return pStack.pushInt(~pStack.popInt());
            case FLOAT:
                return TYPEERROR;
            default:
                return NOTAVALUE;
        }
    
    case TOCHAR:
        switch (pStack.peekByte()){
            case CHAR:
                return 1;
            case INT:
                return pStack.pushChar((char) pStack.popInt());
            case FLOAT:
                return pStack.pushChar((char) pStack.popFloat());
            default:
                return NOTAVALUE;
        }
    
    case TOINT:
        switch (pStack.peekByte()){
            case CHAR:
                return pStack.pushInt((uint8_t) pStack.popChar());
                return 1;
            case INT:
                return 1;
            case FLOAT:
                return pStack.pushInt((int16_t) pStack.popFloat());
            default:
                return NOTAVALUE;
        }

    case TOFLOAT:
        switch (pStack.peekByte()){
            case CHAR:
                return pStack.pushFloat((float) pStack.popChar());
            case INT:
                return pStack.pushFloat((float) pStack.popInt());
            case FLOAT:
                return 1;
            default:
                return NOTAVALUE;
        }

    case ROUND:

    case FLOOR:

    case CEIL:

    case MIN:
        return binaryOperators(id, instruction);

    case MAX:
        return binaryOperators(id, instruction);

    case ABS:

    case CONSTRAIN:

    case MAP:

    case POW:
        return binaryOperators(id, instruction);

    case SQ:

    case SQRT:

    case DELAY:

    case DELAYUNTIL:
        if(pStack.peekInt() <= ((int16_t) millis())){
            pStack.popInt();
            return 1;
        }
        else {
            PC--;
            return 1;
        }

    case MILLIS:
        return pStack.pushInt((int16_t) millis());

    case PINMODE:{
        const uint8_t mode = pStack.popVal();
        pinMode(pStack.popVal(), mode);
        return 1;
    }

    case ANALOGREAD:
        return pStack.pushInt(analogRead(pStack.popVal()));

    case ANALOGWRITE:{
        const float value = pStack.popVal();
        analogWrite(pStack.popVal(), value);
        return 1;
    }

    case DIGITALREAD:
        return pStack.pushChar(digitalRead(pStack.popVal()));

    case DIGITALWRITE:{
        const uint8_t value = pStack.popVal();
        digitalWrite(pStack.popVal(), value);
        return 1;
    }

    case PRINT:
        return print(id);

    case PRINTLN:{
        int16_t r = print(id);
        Serial.println();
        return r;
    }

    case OPEN:{
        const uint16_t size = pStack.popExact();
        char fileName[FILENAMELENGTH];
        pStack.popString(fileName);
        FP = FAT.getAddress(fileName);
        if(FP != 0) return 1;

        FP = FAT.getStoreAddress(size);
        if(FP == 0) return INSUFFICIENTSPACE;
        return FAT.addEntry(fileName, &size, &FP);
    }

    case CLOSE:
        return 1;

    case WRITE:
        return write(id);

    case READINT:
        pStack.pushByte(EEPROM[FP++]);
        pStack.pushByte(EEPROM[FP++]);
        return pStack.pushByte(INT);

    case READCHAR:
        pStack.pushByte(EEPROM[FP++]);
        return pStack.pushByte(CHAR);

    case READFLOAT:
        pStack.pushByte(EEPROM[FP++]);
        pStack.pushByte(EEPROM[FP++]);
        pStack.pushByte(EEPROM[FP++]);
        pStack.pushByte(EEPROM[FP++]);
        return pStack.pushByte(FLOAT);

    case READSTRING:{
        uint8_t len = 0;

        do{
            pStack.pushByte(EEPROM[FP++]);
            len++;
        } while(EEPROM[FP-1] != '\0');

        pStack.pushByte(len);
        return pStack.pushByte(STRING);
    }

    case IF:

    case ELSE:

    case ENDIF:

    case WHILE:
        if(pStack.popVal() == 0){
            PC++;
            PC += pcREAD + 1;
            return 1;
        }
        return pStack.pushByte(pcREAD + pcREAD + 4);

    case ENDWHILE:
        PC -= pStack.popByte();
        return 1;

    case LOOP:
        LR = PC;
        return 1;

    case ENDLOOP:
        PC = LR;
        return 1;

    case STOP:
        memory->eraseAll(id);
        return ptc->terminateProcess(id);
        return 1;

    case FORK:

    case WAITUNTILDONE:

    default:
        Serial.print(F("Unknown instruction: "));
        Serial.println((int) instruction);
        return UNKNOWNINSTRUCTION;
    }


    Serial.print(F("This should be impossible: "));
    Serial.println((int) instruction);
    return 0;
}