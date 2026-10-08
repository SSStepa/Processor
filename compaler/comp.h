#ifndef PROC_H
#define PROC_H

#include <stdio.h>

#include "../stack.h"
#include "../basic.h"

struct commandCode
{
    int code;
    const char *name;
};

/**
 * Command to it's code (sorted in alphabet to bin search)
 */
const commandCode COMMANDS_TO_CODE[] = {
    0,   "HLT",
    1,   "OUT",
    2,   "PUSH",
    3,   "ADD",
    4,   "SUBTR",
    5,   "MULT",
    6,   "DIV",
    7,   "SQRT",
    8,   "SIN",
    9,   "COS",
    10,  "TAN",
    -1, ""
};

const int COMMAND_MAX_LEN = 20;




WORK_RES GetCommand(int *command, double *arg);

#endif