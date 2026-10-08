#ifndef PROC_H
#define PROC_H

#include <math.h>
#include <sys/stat.h>

#include "../stack.h"
#include "../basic.h"

const size_t STACK_INIT_CAPACITY = 10;

WORK_RES MainCalculation(const char *FileName);

WORK_RES GetCompiledFileInfo(const char *fileName, string_t *statusCode);

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