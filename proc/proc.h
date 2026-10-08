#ifndef PROC_H
#define PROC_H

#include <math.h>

#include "../stack.h"
#include "../basic.h"

const size_t STACK_INIT_CAPACITY = 10;

/**
 * used to check that function run correctly
 * 
 * need to be variable statusCode initialized.
 */
#define RUN_COMMAND(func, errorMessage)           \
    if ((statusCode = func) != OK)                \
        return $ERR(errorMessage, statusCode)



WORK_RES MainCalculation(const char *FileName);

double *GetCompiledFileInfo(const char *fileName, WORK_RES *statusCode);

WORK_RES StopWork(double *commandLine, stack_t *stk);

WORK_RES ShowRes(stack_t *stk);

WORK_RES AddElls(stack_t *stk);
WORK_RES SubtrElls(stack_t *stk);
WORK_RES MultElls(stack_t *stk);
WORK_RES DivElls(stack_t *stk);
WORK_RES SqrtEll(stack_t *stk);
WORK_RES SinEll(stack_t *stk);
WORK_RES CosEll(stack_t *stk);
WORK_RES TanEll(stack_t *stk);
#endif;