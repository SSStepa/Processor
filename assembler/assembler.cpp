#include "assembler.h"

WORK_RES Assembler(const char *inFileName, const char *outBinFileName, const char *outFileName)
{
    assert(inFileName);
    assert(outFileName);

    WORK_RES statusCode = OK;

    FileData data = {};

    RUN_COMMAND(GetFileFull(inFileName, &data.asmStartData), "ERROR WHILE TRYING To GET INFO FROM FILE");
    RUN_COMMAND(CountEls(&data.indLen, data.asmStartData.str, data.asmStartData.len, '\n'), "ERROR WHILE COUNTING AMOUNT OF LINES");
    RUN_COMMAND(ParseFileToLines(&data), "CAN'T PARSE GOT ASM INTO LINES");

    RUN_COMMAND(LabelsInit(&data.labelsList), "ERROR WHILE TRYING TO INIT LABEL LIST");

    bool  firstRun = true;
    RUN_COMMAND(GetCompiledCodeLine(&data, firstRun), "ERROR WHILE FIRST ASSEMBLER");

    firstRun = false;
    free(data.binOpcode.str);
    free(data.textOpcode.str);
    RUN_COMMAND(GetCompiledCodeLine(&data, firstRun), "ERROR WHILE SECOND ASSEMBLER");
    
    RUN_COMMAND(OutFileFull(outBinFileName, outFileName, &data), "ERROR WHILE TRYING TO WRITE INTO OUT FILE");
    RUN_COMMAND(ClearData(&data), "ERROR WHILE TRYING TO CLEAR DATA");

    return statusCode;
}

WORK_RES GetCompiledCodeLine(FileData *data, bool firstRun)
{
    assert(data);

    WORK_RES statusCode = OK;

    (data -> textOpcode).str = (char *) calloc((data -> asmStartData).len, sizeof(char));
    if ((data -> textOpcode).str == NULL) return $ERR("NOT ENOUGH MEMORY WHILE CREATING BUFFER FOR COMPILED COMMANDS AS TEXT", NOMEM);
    (data -> textOpcode).len = 0;

    (data -> binOpcode).str = (char *) calloc((data -> asmStartData).len*8, sizeof(char)); // TODO: controle size 
    if ((data -> binOpcode).str == NULL) return $ERR("NOT ENOUGH MEMORY WHILE CREATING BUFFER FOR COMPILED COMMANDS AS TEXT", NOMEM);
    (data -> binOpcode).len = 0;
    
    char command[COMMAND_MAX_LEN] = "";
    int elementsAmount = 0;

    for (size_t ind = 0; ind < data -> indLen; ind++)
    {
        data -> commandLine = (data -> indexDyn)[ind].str;

        int step = 0;

        if (sscanf(data -> commandLine, "%s%n", command, &step) <= 0) continue;
        data -> commandLine += step;

        if (command[0] == ';') continue;

        if (command[0] == ':') {
            if (firstRun)
                RUN_COMMAND(PutLabelIntoList(&(data -> labelsList), command, elementsAmount), "ERROR WHILE TRYING TO PUT LABEL");
            continue;
        }
        
        statusCode = PutCommandToCode(data, command, &elementsAmount);
        if (statusCode) {
            if (statusCode == NO_LABEL && firstRun) statusCode = OK;
            else return $ERR("ERROR WHILE TRYING TO CONVERT COMMAND", statusCode);
        }
    }

    return statusCode;
}

WORK_RES PutCommandToCode(FileData *data, char *command, int *elementsAmount)
{
    assert(data);
    assert(command);
    assert(elementsAmount);

    WORK_RES statusCode = OK;

    int commandNumber = 0;
    
    while(strcmp(COMMANDS_TO_CODE[commandNumber].name, "") != 0) { 
        if (strcmp(command, COMMANDS_TO_CODE[commandNumber].name) == 0) {

            if (commandNumber == PUSH) {
                RUN_COMMAND(PushCommandToCode(data, elementsAmount), "WRONG WHILE TRYING TO CONVERT PUSH");

            } else if (commandNumber == POP) {
                RUN_COMMAND(PopCommandToCode(data, elementsAmount), "WRONG WHILE TRYING TO POP");    

            } else if (
                    commandNumber == JMP || commandNumber == JA  || commandNumber == JB  ||
                    commandNumber == JE  || commandNumber == JNE || commandNumber == JAE ||
                    commandNumber == JBE || commandNumber == JT ||
                    commandNumber == CALL
                    ) {
                (data -> textOpcode).len += sprintf((data -> textOpcode).str + (data -> textOpcode).len, "%d ", commandNumber);
                *((data -> binOpcode).str + (data -> binOpcode).len++) =  (char) commandNumber;
                statusCode = GoToArgToCode(data);
                *elementsAmount += sizeof(double);
            
            } else {
                (data -> textOpcode).len += sprintf((data -> textOpcode).str + (data -> textOpcode).len, "%d", commandNumber);
                *((data -> binOpcode).str + (data -> binOpcode).len++) = (char) commandNumber;
            }


            sprintf(&((data -> textOpcode).str[(data -> textOpcode).len]), "\n");
            (data -> textOpcode).len += 1;
            break;
        }

        commandNumber++;
    }
    if (strcmp(COMMANDS_TO_CODE[commandNumber].name, "") == 0)
        return $ERR("NO SUCH COMMAND TO COMPILE", WRIN);

    (*elementsAmount)++;

    return statusCode;
}

WORK_RES PushCommandToCode(FileData *data, int *elementsAmount)
{
    assert(data);
    assert(elementsAmount);

    char commandNumber = (char) PUSH;

    PushPopArgParse(&commandNumber, data, elementsAmount);

    return OK;
}

WORK_RES PopCommandToCode(FileData *data, int *elementsAmount)
{
    assert(data);
    assert(elementsAmount);

    char commandNumber = (char) POP;

    PushPopArgParse(&commandNumber, data, elementsAmount); 

    if (commandNumber == 0X23 || commandNumber == 0x63) {
        return $ERR("SYNTAX ERROR: WRONG ARGUMENT FOR POP", WRIN);
    }

    return OK;
}

WORK_RES GetRegisterByName(char *command, char *regNumber)
{
    assert(command);
    assert(regNumber);

    size_t regInd = 0;
    while (strcmp(REGISTERS[regInd].name, "") != 0) {
        if (strcmp(REGISTERS[regInd].name, command) == 0) {
            *regNumber = REGISTERS[regInd].code;
            break; 
        }
        regInd++;
    }
    if (strcmp(REGISTERS[regInd].name, "") == 0)
        return $ERR("NO SUCH REGISTER NAME FOR POP", WRIN);
    
    return OK;
}

WORK_RES PushPopArgParse(char *commandNumber, FileData *data, int *elementsAmount)
{
    assert(commandNumber);
    assert(data);

    WORK_RES statusCode = OK;

    double arg1 = NAN;
    char   arg2[REGISTER_MAX_LEN] = "";

    unsigned char commandAdd = 0;
    (data -> commandLine)++; // skip whitespace

    if ((data -> commandLine)[0] == '[') {
        commandAdd |= 0x80;
        (data -> commandLine)++; 
    }

    if (sscanf(data -> commandLine, "%lg + %3[^] ]", &arg1, arg2) == 2 ||
        sscanf(data -> commandLine, "%3[^+ ] + %lg",  arg2, &arg1) == 2) {
        commandAdd |= 0x60;
    } else if (sscanf(data -> commandLine, "%lg", &arg1) == 1) {
        commandAdd |= 0x20;
        *elementsAmount += sizeof(double);
    } else if (sscanf(data -> commandLine, "%3[^] ]", arg2) == 1) {
        commandAdd |= 0x40;
        (*elementsAmount)++;
    } else return $ERR("WRONG INPUT WHILE GETING PUSH ARGUMENT", WRIN);

    *commandNumber |= commandAdd;
    (data -> textOpcode).len += sprintf((data -> textOpcode).str + (data -> textOpcode).len, "%d ", *commandNumber);
    *((data -> binOpcode).str + (data -> binOpcode).len++) =  *commandNumber;

    if (commandAdd & 0x20) {
        (data -> textOpcode).len += sprintf((data -> textOpcode).str + (data -> textOpcode).len, "%lg", arg1);
        *((double *) ((data -> binOpcode).str + (data -> binOpcode).len)) =  arg1;
        (data -> binOpcode).len += sizeof(double);
    }
    if (commandAdd & 0x40) {
        char regCode = 0;
        RUN_COMMAND(GetRegisterByName(arg2, &regCode), "WRONG NAME OF THE REGISTER");

        (data -> textOpcode).len += sprintf((data -> textOpcode).str + (data -> textOpcode).len, " %d", regCode);
        *((data -> binOpcode).str + (data -> binOpcode).len++) =  regCode;
    }
    
    return OK;
}

WORK_RES GoToArgToCode(FileData *data)
{
    assert(data);

    WORK_RES statusCode = OK;

    (data -> commandLine)++; // skip whitespace
    
    char label[MAX_LABEL_LEN] = "";
    int goTo = -1;
    if ((data -> commandLine)[0] == ':') {
        sscanf((data -> commandLine), ":%s", label);

        size_t ind = 0;
        for (ind = 0; ind < (data -> labelsList).len; ind ++) {
            if (strcmp((data -> labelsList).labels[ind].name, label) == 0) {
                goTo = (data -> labelsList).labels[ind].placeToGo;
                break;
            }
        } 
        if (ind == (data ->labelsList).len)
            goTo = -1;

    } else
        if (sscanf(data -> commandLine, "%d", &goTo) != 1) return $ERR("WRONG ARGUMENT FOR JUMP COMMAND", WRIN);
    
    (data -> textOpcode).len += sprintf((data -> textOpcode).str + (data -> textOpcode).len, "%lg", (double) goTo);
    *((double *) ((data -> binOpcode).str + (data -> binOpcode).len)) =  (double) goTo;
    (data -> binOpcode).len += sizeof(double);

    return statusCode;

}

WORK_RES PutLabelIntoList(labelList_t *labelList, char *command, int elementInd)
{
    assert(labelList);
    assert(command);

    WORK_RES statusCode = OK;

    strcat((labelList -> labels)[(labelList -> fullElls)].name, &command[1]);
    (labelList -> labels)[labelList -> fullElls].placeToGo = elementInd;
    (labelList -> fullElls)++;

    if (labelList -> fullElls >= labelList -> len) {
        labelList -> len *= 2;

        label_t *temp = (label_t *)  realloc(labelList -> labels, sizeof(label_t) * (labelList -> len));
        if (temp == NULL) 
            return $ERR("NOT ENOUGH MEMORY TO RESIZE LABELS", NOMEM);

        labelList -> labels = temp;
    }

    return statusCode;
    
}

WORK_RES LabelsInit(labelList_t *labelList)
{
    assert(labelList);

    WORK_RES statusCode = OK;

    labelList -> labels = (label_t *) calloc(LABELS_START_SIZE, sizeof(label_t));
    if (labelList -> labels == NULL)
        return $ERR("NOT ENOUGH MEMORY FOR LABELS", NOMEM);
    
    labelList -> len = LABELS_START_SIZE;

    RUN_COMMAND(InitLabelsValues(labelList -> labels, labelList -> len), "ERROR WHILE TRYING TO INIT LABELS");

    return statusCode;
}

WORK_RES InitLabelsValues(label_t *labels, size_t len)
{
    assert(labels);

    for (size_t ind = 0; ind < len; ind++) {
        strcat(labels[ind].name, LABEL_INIT);
        labels[ind].placeToGo = -1;
    }
    
    return OK;
}

WORK_RES ParseFileToLines(FileData *data)
{
    assert(data);
    
    data -> indexDyn = (string_t *) calloc(data -> indLen, sizeof(string_t));
    if (data -> indexDyn == NULL) 
        return $ERR("NOT ENOUGH MEMORY FOR INDEX", NOMEM);
        
    *(data -> indexDyn) = {.str = (data -> asmStartData).str};
    data -> indLen = 1;
    size_t lineLen = 1;

    for (size_t ind = 0; ind < data -> asmStartData.len; ind++, lineLen++){
        if (((data -> asmStartData).str)[ind] == '\n') {
            ((data -> asmStartData).str)[ind] = '\0';
            
            ((data -> indexDyn) + (data -> indLen) - 1) -> len = lineLen;
            lineLen = 0;

            (data -> indexDyn + (data -> indLen)++) -> str = &((data -> asmStartData.str)[ind + 1]);
        }
    }
    (data -> indexDyn + data -> indLen - 1) -> len = lineLen;

    return OK;
}

WORK_RES CountEls(size_t *amount, char *arr, size_t  arrLen, char El)
{
    assert(arr != 0);
    
    char *indLine = arr;
    while (indLine != 0 && indLine < (arr + arrLen)) {
        indLine = strchr(indLine + 1, El);
        (*amount)++;
    }

    return OK;
}

WORK_RES ClearData(FileData *data)
{
    assert(data);

    free((data -> asmStartData).str);
    free((data -> binOpcode).str);
    free((data -> textOpcode).str);
    free(data ->  indexDyn);


    return OK;

}

WORK_RES OutFileFull(const char *outBinName, const char *outFileTextName, FileData *data)
{
    assert(outBinName);
    assert(outFileTextName);
    assert(data);


    int fileDes = open(outFileTextName, O_WRONLY, 0);
    if (fileDes == -1) return $ERR("NO FILE TO WRITE TO IN OutFileFull", FILEERR);

    write(fileDes, (data -> textOpcode).str, (unsigned int) (data -> textOpcode).len);

    close(fileDes);

    FILE *binFile = fopen(outBinName, "wb");
    if (binFile == NULL) return $ERR("NO FILE TO WRITE TO BIN IN OutFileFull", FILEERR);

    fwrite((data -> binOpcode).str, sizeof(char), (data -> binOpcode).len, binFile);

    fclose(binFile);
    
    return OK;
}
