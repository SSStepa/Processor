#include "assembler.h"

WORK_RES Assembler(const char *inFileName, const char *outFileName)
{
    assert(inFileName);
    assert(outFileName);

    WORK_RES statusCode = OK;

    string_t commandAsmData = {};

    RUN_COMMAND(GetFileFull(inFileName, &commandAsmData), "ERROR WHILE TRYING To GET INFO FROM FILE");

    string_t commandByteCodeData = {};
    
    labelList_t labelsList = {};
    RUN_COMMAND(LabelsInit(&labelsList), "ERROR WHILE TRYING TO INIT LABEL LIST");

    bool  firstRun = true;
    RUN_COMMAND(GetCompiledCodeLine(&commandAsmData, &commandByteCodeData, firstRun, &labelsList), "ERROR WHILE FIRST ASSEMBLER");

    firstRun = false;
    free(commandByteCodeData.str);
    RUN_COMMAND(GetCompiledCodeLine(&commandAsmData, &commandByteCodeData, firstRun, &labelsList), "ERROR WHILE SECOND ASSEMBLER");
    

    RUN_COMMAND(OutFileFull(outFileName, &commandByteCodeData), "ERROR WHILE TRYING TO WRITE INTO OUT FILE");

    free(commandAsmData.str);
    free(commandByteCodeData.str);
    free(labelsList.labels);

    return statusCode;
}

WORK_RES GetCompiledCodeLine(string_t *inLine, string_t *outLine, bool firstRun, labelList_t *labelList)
{
    assert(inLine);
    assert(outLine);

    WORK_RES statusCode = OK;

    outLine -> str = (char *) calloc(inLine -> len, sizeof(char));
    if (outLine -> str == NULL) return $ERR("NOT ENOUGH MEMORY WHILE CREATING BUFFER FOR COMPILED COMMANDS", NOMEM);
    
    size_t ind = 0;
    char command[COMMAND_MAX_LEN] = "";
    int elementsAmount = 0;

    while (ind < inLine -> len) {
        size_t step = 0;

        if (sscanf((inLine -> str) + ind, "%s%n", command, &step) <= 0) break;
        ind += step;

        if (command[0] == ':') {
            if (firstRun)
                RUN_COMMAND(PutLabelIntoList(labelList, command, elementsAmount), "ERROR WHILE TRYING TO PUT LABEL");
            continue;
        }
        
        statusCode = PutCommandToCode(inLine -> str, outLine -> str, command, &ind, &elementsAmount, labelList);
        if (statusCode) {
            if (statusCode == NO_LABEL && firstRun) statusCode = OK;
            else return $ERR("ERROR WHILE TRYING TO CONVERT COMMAND", statusCode);
        }
    }

    outLine -> len = strlen(outLine -> str);
    outLine -> str = (char *) realloc(outLine -> str, outLine -> len + 1);

    return statusCode;
}

WORK_RES PutCommandToCode(char *inLine, char *outLine, char *command, size_t *ind, int *elementsAmount, labelList_t *labelList)
{
    assert(inLine);
    assert(outLine);
    assert(command);
    assert(ind);
    assert(elementsAmount);

    WORK_RES statusCode = OK;

    size_t commandNumber = 0;
    while(strcmp(COMMANDS_TO_CODE[commandNumber].name, "") != 0) { 
        if (strcmp(command, COMMANDS_TO_CODE[commandNumber].name) == 0) {

            if (commandNumber == PUSH) {
                RUN_COMMAND(PushCommandToCode(inLine, outLine, ind), "WRONG WHILE TRYING TO CONVERT PUSH");
                (*elementsAmount)++;

            } else if (commandNumber == POP) {
                RUN_COMMAND(PopCommandToCode(inLine, outLine, ind), "WRONG WHILE TRYING TO POP");    
                (*elementsAmount)++;

            } else if (
                    commandNumber == JMP || commandNumber == JA  || commandNumber == JB  ||
                    commandNumber == JE  || commandNumber == JNE || commandNumber == JAE ||
                    commandNumber == JBE || commandNumber == JT ||
                    commandNumber == CALL
                    ) {
                strcat(outLine, COMMANDS_TO_CODE[commandNumber].code);
                printf("<%s> %d", COMMANDS_TO_CODE[commandNumber].code, commandNumber);
                statusCode = GoToArgToCode(inLine, outLine, ind, labelList);
                (*elementsAmount)++;
            
            } else {
            strcat(outLine, COMMANDS_TO_CODE[commandNumber].code);
            }


            strcat(outLine, "\n");
            break;
        }

        commandNumber++;
    }
    if (strcmp(COMMANDS_TO_CODE[commandNumber].name, "") == 0)
        return $ERR("NO SUCH COMMAND TO COMPILE", WRIN);

    (*elementsAmount)++;

    return statusCode;
}

WORK_RES PushCommandToCode(char *inLine, char *outLine, size_t *ind)
{
    assert(inLine);
    assert(outLine);
    assert(ind);

    size_t step = 0;
    double arg = 0;
    char command[COMMAND_MAX_LEN] = {};

    if (sscanf(inLine + *ind, "%lg%n", &arg, &step) > 0) {
        strcat(outLine, "2");
        sprintf(command, "%lg", arg);

    } else {
        sscanf(inLine + *ind, "%s%n", &command, &step);
        strcat(outLine, "67");

        size_t regInd = 0;
        while (strcmp(REGISTERS[regInd].name, "") != 0) {
            if (strcmp(REGISTERS[regInd].name, command) == 0) {
                sprintf(command, "%s", REGISTERS[regInd].code);
                break; 
            }
            regInd++;
        }
        if (strcmp(REGISTERS[regInd].name, "") == 0)
            return $ERR("NO SUCH REGISTER NAME FOR PUSH", WRIN);
    }

    *ind += step;
    strcat(outLine, " ");
    strcat(outLine, command);

    return OK;
}

WORK_RES PopCommandToCode(char *inLine, char *outLine, size_t *ind)
{
    assert(inLine);
    assert(outLine);
    assert(ind);

    strcat(outLine, COMMANDS_TO_CODE[POP].code);

    char command[COMMAND_MAX_LEN] = {};
    size_t step = 0;

    sscanf(inLine + *ind, "%s%n", &command, &step);
    *ind += step;

    size_t regInd = 0;
    while (strcmp(REGISTERS[regInd].name, "") != 0) {
        if (strcmp(REGISTERS[regInd].name, command) == 0) {
            sprintf(command, "%s", REGISTERS[regInd].code);
            break; 
        }
        regInd++;
    }
    if (strcmp(REGISTERS[regInd].name, "") == 0)
        return $ERR("NO SUCH REGISTER NAME FOR POP", WRIN);

    strcat(outLine, " ");
    strcat(outLine, command);

    return OK;
}

WORK_RES GoToArgToCode(char *inLine, char *outLine, size_t *ind, labelList_t *labelList)
{
    assert(inLine);
    assert(outLine);
    assert(ind);

    WORK_RES statusCode = OK;

    char command[COMMAND_MAX_LEN] = {};
    size_t step = 0;

    sscanf(inLine + *ind, "%s%n", &command, &step);
    
    int label = 0;
    if (command[0] == ':') {
        sscanf(command, ":%d", &label);
        if (label < 0) return $ERR("ERROR LABEL HAVE TO BE POSITIVE", WRIN);

        if (label >= labelList -> len || labelList -> labels[label] == -1) {
            sprintf(command, "%d", -1);
            statusCode = NO_LABEL;
        } else
            sprintf(command, "%d", labelList -> labels[label]);

    } else
        if (sscanf(command, "%d", &label) != 1) return $ERR("WRONG ARGUMENT FOR JUMP COMMAND", WRIN);
    
    *ind += step;
    strcat(outLine, " ");
    strcat(outLine, command);

    return statusCode;

}

WORK_RES PutLabelIntoList(labelList_t *labelList, char *command, int elementInd)
{
    assert(labelList);
    assert(command);

    WORK_RES statusCode = OK;

    int  label = 0;
    if (sscanf(command, ":%d", &label) != 1)
        return $ERR("CAN'T GET LABEL FROM ASM", WRIN);
    
    if (label < 0)
        return $ERR("LABELS HAVE TO BE GREATER THAN ZERO", WRIN);
    
    if (label >= labelList -> len) {
        int *temp = (int *)  realloc(labelList -> labels, sizeof(int) * (label + 1));
        if (temp == NULL) 
            return $ERR("NOT ENOUGH MEMORY TO RESIZE LABELS", NOMEM);

        labelList -> labels = temp;
        RUN_COMMAND(InitLabelsValues(labelList -> labels + labelList -> len, label - labelList -> len + 1), "ERROR WHILE TRYING TO INIT LABELS VALUES");
        labelList -> len = label + 1;
    }

    if (labelList -> labels[label] != LABEL_INIT)
        return $ERR("REINIT OF LABEL", WRIN);
    
    labelList -> labels[label] = elementInd;

    return statusCode;
    
}

WORK_RES LabelsInit(labelList_t *labelList)
{
    assert(labelList);

    WORK_RES statusCode = OK;

    labelList -> labels = (int *) calloc(LABELS_START_SIZE, sizeof(int));
    if (labelList -> labels == NULL)
        return $ERR("NOT ENOUGH MEMORY FOR LABELS", NOMEM);
    
    labelList -> len = LABELS_START_SIZE;

    RUN_COMMAND(InitLabelsValues(labelList -> labels, labelList -> len), "ERROR WHILE TRYING TO INIT LABELS");

    return statusCode;
}

WORK_RES InitLabelsValues(int *labels, size_t len)
{
    assert(labels);

    for (size_t ind = 0; ind < len; ind++) {
        labels[ind] = LABEL_INIT;
    }
    
    return OK;
}
