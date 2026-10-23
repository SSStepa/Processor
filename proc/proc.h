#ifndef PROC_H
#define PROC_H

#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <windows.h>

#include "../stack.h"
#include "../basic.h"

const size_t RAM_INIT_SIZE = 3495;
const size_t STACK_INIT_CAPACITY = 10;
const double SLEEP_TIME = 1000/27;

struct OpCode {
    unsigned int code  :5;
    int konst :1;
    int reg   :1;
    int mem   :1;
};

struct SPU_t {
    stack_t dataStk;
    stack_t callStk;
    double registers[5];

    char *RAM;
    
    char *code;
    size_t codeLen;
    size_t IP;

    OpCode command;
};

WORK_RES Proc(const char *FileName);

WORK_RES SPU_Init(SPU_t *spu, const char *fileName);

WORK_RES GetCompiledFileInfo(const char *fileName, char **code, size_t *codeLen);

WORK_RES StopWork(SPU_t *spu);

WORK_RES ShowResAsChar(stack_t *stk);
WORK_RES ShowRes(stack_t *stk);
WORK_RES InCommand(SPU_t *spu);
WORK_RES DrawRAMCommand(SPU_t *spu);

WORK_RES PushElls(SPU_t *spu);
WORK_RES PopElls(SPU_t *spu);

WORK_RES AddElls(stack_t *stk);
WORK_RES SubtrElls(stack_t *stk);
WORK_RES MultElls(stack_t *stk);
WORK_RES DivElls(stack_t *stk);
WORK_RES SqrtEll(stack_t *stk);
WORK_RES SinEll(stack_t *stk);
WORK_RES CosEll(stack_t *stk);
WORK_RES TanEll(stack_t *stk);

WORK_RES JmpCommand(SPU_t *spu);
WORK_RES JaCommand(SPU_t *spu);
WORK_RES JbCommand(SPU_t *spu);
WORK_RES JeCommand(SPU_t *spu);
WORK_RES JaeCommand(SPU_t *spu);
WORK_RES JbeCommand(SPU_t *spu);
WORK_RES JneCommand(SPU_t *spu);
WORK_RES JtCommand(SPU_t *spu);

WORK_RES CallFunctionCommand(SPU_t *spu);
WORK_RES RetFunctionCommand(SPU_t *spu);

#define JMP_CREATE(funcName, operationEl1El2)                                                 \
WORK_RES funcName(SPU_t *spu)                                                           \
{                                                                                       \
    assert(spu);                                                                        \
                                                                                        \
    WORK_RES statusCode = OK;                                                           \
                                                                                        \
    double el1 = 0;                                                                     \
    double el2 = 0;                                                                     \
                                                                                        \
    RUN_COMMAND(StackPop(&(spu -> dataStk), &el1), "CAN'T GET ELEMENT FROM STACK");     \
    RUN_COMMAND(StackPop(&(spu -> dataStk), &el2), "CAN'T GET ELEMENT FROM STACK");     \
                                                                                        \
    if (operationEl1El2)                                                 \
        spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));                   \
    else                                                                                \
        spu -> IP += sizeof(double);                                                    \
                                                                                        \
    return OK;                                                                          \
}

#endif