#ifndef DECOMP_H
#define DECOMP_H

#include <stdio.h>
#include <sys/stat.h>

#include "../basic.h"
#include "../stack.h"

struct commandCode
{
    const char *code;
    const char *name;
};

const int COMMAND_MAX_LEN = 10;

/**
 * Commands sorted as code is index(fast decompilation)
 */
const commandCode CODE_TO_COMMANDS[] = {
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
    "",    ""
};

WORK_RES DeCompiler(const char *inFileName, const char *outFileName);
WORK_RES GetDecompiledCodeLine(string_t *inLine, string_t *outLine);

WORK_RES GetCodeFileFull(const char *fileName, string_t *string);
WORK_RES OutAsmCodeFile (const char *fileName, string_t *string);

#endif
