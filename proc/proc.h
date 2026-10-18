#ifndef PROC_H
#define PROC_H

#include <math.h>
#include <sys/stat.h>

#include "../stack.h"
#include "../basic.h"

const size_t RAM_INIT_SIZE = 100;
const size_t STACK_INIT_CAPACITY = 10;

struct OpCode {
    int code  :5;
    int konst :1;
    int reg   :1;
    int mem   :1;
};

struct SPU_t {
    stack_t dataStk;
    stack_t callStk;
    double registers[5];

    double RAM[RAM_INIT_SIZE];
    
    char *code;
    size_t codeLen;
    size_t IP;
};


WORK_RES Proc(const char *FileName);

WORK_RES SPU_Init(SPU_t *spu, const char *fileName);

WORK_RES GetCompiledFileInfo(const char *fileName, char *code, size_t *codeLen);

WORK_RES StopWork(string_t *commandLine, stack_t *stk);

WORK_RES ShowRes(stack_t *stk);

WORK_RES AddElls(stack_t *stk);
WORK_RES SubtrElls(stack_t *stk);
WORK_RES MultElls(stack_t *stk);
WORK_RES DivElls(stack_t *stk);
WORK_RES SqrtEll(stack_t *stk);
WORK_RES SinEll(stack_t *stk);
WORK_RES CosEll(stack_t *stk);
WORK_RES TanEll(stack_t *stk);
#endif