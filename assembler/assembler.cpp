#include "assembler.h"

WORK_RES Assembler(const char *inFileName, const char *outFileName)
{
    assert(inFileName);
    assert(outFileName);

    WORK_RES statusCode = OK;

    string_t commandAsmData = {};

    RUN_COMMAND(GetFileFull(inFileName, &commandAsmData), "ERROR WHILE TRYING To GET INFO FROM FILE");

    string_t commandByteCodeData = {};

    bool badLabels = false;
    RUN_COMMAND(GetCompiledCodeLine(&commandAsmData, &commandByteCodeData, &badLabels), "ERROR WHILE FIRST ASSEMBLER");

    if (badLabels) {
        RUN_COMMAND(GetCompiledCodeLine(&commandAsmData, &commandByteCodeData, &badLabels), "ERROR WHILE SECOND ASSEMBLER")
    }

    RUN_COMMAND(OutFileFull(outFileName, &commandByteCodeData), "ERROR WHILE TRYING TO WRITE INTO OUT FILE");

    free(commandAsmData.str);
    free(commandByteCodeData.str);

    return statusCode;
}

WORK_RES GetCompiledCodeLine(string_t *inLine, string_t *outLine, bool *badLabels)
{
    assert(inLine);
    assert(outLine);

    WORK_RES statusCode = OK;

    outLine -> str = (char *) calloc(inLine -> len, sizeof(char));
    if (outLine -> str == NULL) return $ERR("NOT ENOUGH MEMORY WHILE CREATING BUFFER FOR COMPILED COMMANDS", NOMEM);
    
    size_t ind = 0;
    char command[COMMAND_MAX_LEN] = "";

    while (ind < inLine -> len) {
        size_t step = 0;

        if (sscanf((inLine -> str) + ind, "%s%n", command, &step) <= 0) break;
        ind += step;

        size_t commandNumber = 0;
        while(strcmp(COMMANDS_TO_CODE[commandNumber].name, "") != 0) { // TODO: Sort and add bin search
            if (strcmp(command, COMMANDS_TO_CODE[commandNumber].name) == 0) {

                if (commandNumber == PUSH) {
                    RUN_COMMAND(PushCommandToCode(inLine -> str, outLine -> str, &ind), "WRONG WHILE TRYING TO CONVERT PUSH");

                } else if (commandNumber == POP) {
                    RUN_COMMAND(PopCommandToCode(inLine -> str, outLine -> str, &ind), "WRONG WHILE TRYING TO POP");    

                } else {
                strcat(outLine -> str, COMMANDS_TO_CODE[commandNumber].code);
                }

                strcat(outLine -> str, "\n");
                break;
            }

            commandNumber++;
        }
        if (strcmp(COMMANDS_TO_CODE[commandNumber].name, "") == 0)
            return $ERR("NO SUCH COMMAND TO COMPILE", WRIN);

    }

    outLine -> len = strlen(outLine -> str);
    outLine -> str = (char *) realloc(outLine -> str, outLine -> len + 1);

    return statusCode;
}

WORK_RES PushCommandToCode(char *inLine, char *outLine, size_t *ind)
{
    assert(inLine);
    assert(outLine);

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

    return OK;
}