#ifndef DECOMP_H
#define DECOMP_H

struct commandCode
{
    int code;
    const char *name;
};

/**
 * Commands sorted as code is index(fast decompilation)
 */
const commandCode CODE_TO_COMMANDS[] = {
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

#endif
