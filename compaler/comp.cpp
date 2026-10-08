#include "comp.h"

WORK_RES Compiler(const char *inFileName, const char *outFileName)
{
    assert(inFileName);
    assert(outFileName);

    WORK_RES statusCode = OK;

    string_t commandAsmData = {};

    RUN_COMMAND(GetAsmFileFull(inFileName, &commandAsmData), "ERROR WHILE TRYING To GET INFO FROM FILE");

    string_t commandByteCodeData = {};

    RUN_COMMAND(GetCompiledCodeLine(&commandAsmData, &commandByteCodeData), "ERROR WHILE COMPILATION");

    RUN_COMMAND(OutByteCodeFile(outFileName, &commandByteCodeData), "ERROR WHILE TRYING TO WRITE INTO OUT FILE");

    free(commandAsmData.str);
    free(commandByteCodeData.str);

    return statusCode;
}

WORK_RES GetCompiledCodeLine(string_t *inLine, string_t *outLine)
{
    assert(inLine);
    assert(outLine);

    outLine -> str = (char *) calloc(inLine -> len, sizeof(char));
    if (outLine -> str == NULL) return $ERR("NOT ENOUGH MEMORY WHILE CREATING BUFFER FOR COMPILED COMMANDS", NOMEM);
    
    size_t ind = 0;
    char command[COMMAND_MAX_LEN] = "";

    while (ind <= inLine -> len) {
        size_t step = 0;

        if (sscanf((inLine -> str) + ind, "%s%n", command, &step) <= 0) break;
        ind += step;

        size_t commandNumber = 0;
        while(strcmp(COMMANDS_TO_CODE[commandNumber].name, "") != 0) { // TODO: Sort and add bin search

            if (strcmp(command, COMMANDS_TO_CODE[commandNumber].name) == 0) {
                strcat(outLine -> str, COMMANDS_TO_CODE[commandNumber].code);
                
                if (strcmp(command, "PUSH") == 0) {
                    strcat(outLine -> str, " ");
                    sscanf((inLine -> str) + ind, "%s%n", command, &step);
                    ind += step;

                    strcat(outLine -> str, command);
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

    return OK;
}

WORK_RES GetAsmFileFull(const char *fileName, string_t *string)
{
    assert(fileName != NULL);
    assert(string);

    int fileDes = open(fileName, O_RDONLY, 0);
    if (fileDes == -1) return $ERR("NO FILE TO READ FROM IN GetFAsmFileFull", FILEERR);

    struct stat st = {};
    if (fstat(fileDes, &st)) return $ERR("ERROR IN FILE WHILE TRYING TO fstat", FILEERR);

    string -> str = (char *) calloc(st.st_size + 1, sizeof(char));
    if (string -> str == NULL) return $ERR("NOT ENOUGH MEMORY TO READ FILE AS FULL", NOMEM);

    int fileLen = read(fileDes, (void *) string -> str, st.st_size);
    if (fileLen < 0) return $ERR("ERROR WHILE READING FROM FILE", FILEERR);

    string -> str = (char *) realloc(string -> str, fileLen + 1);
    *(string -> str + fileLen) = '\0';
    
    string -> len = (size_t) fileLen;

    close(fileDes);

    return OK;
}

WORK_RES OutByteCodeFile(const char *fileName, string_t *string)
{
    assert(fileName);
    assert(string);


    int fileDes = open(fileName, O_WRONLY, 0);
    if (fileDes == -1) return $ERR("NO FILE TO WRITE TO IN OutByteCodeFile", FILEERR);

    write(fileDes, string -> str, string ->len);

    close(fileDes);
    
    return OK;
}