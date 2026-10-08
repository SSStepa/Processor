#ifndef PROC_H
#define PROC_H

#include <stdio.h>
#include <sys/stat.h>

#include "../stack.h"
#include "../basic.h"

struct commandCode
{
    const char *code;
    const char *name;
};

/**
 * Command to it's code (sorted in alphabet to bin search)
 */
const commandCode COMMANDS_TO_CODE[] = {
    "0",   "HLT",
    "1",   "OUT",
    "2",   "PUSH",
    "3",   "ADD",
    "4",   "SUBTR",
    "5",   "MULT",
    "6",   "DIV",
    "7",   "SQRT",
    "8",   "SIN",
    "9",   "COS",
    "10",  "TAN",
    "",  ""
};

const int COMMAND_MAX_LEN = 20;


WORK_RES Compiler(const char *inFileName, const char *outFileName);
WORK_RES GetCompiledCodeLine(string_t *inLine, string_t *outLine);

WORK_RES GetAsmFileFull (const char *fileName, string_t *string);
WORK_RES OutByteCodeFile(const char *fileName, string_t *string);

WORK_RES GetCommand(int *command, double *arg);

#endif