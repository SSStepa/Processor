#include "proc.h"


WORK_RES Proc(const char *FileName)
{
    assert(FileName);


    WORK_RES statusCode = OK;

    SPU_t spu = {};

    SPU_Init(&spu, FileName);

    int command = -1;
    stackDataType val = 0;

    $llu(spu.codeLen);
    for (int i = 0; i < spu.codeLen; i++)
        $dbl(spu.code[i]);

    return OK;

    // while (ind <= commandLine.len) {
    //     size_t step = 0;

    //     if (sscanf(commandLine.str + ind, "%d%n", &command, &step) <= 0) break;
    //     ind += step;

    //     switch (command) {
    //     case 0:
    //         return StopWork(&commandLine, &spu);
    //     case 1:
    //         RUN_COMMAND(ShowRes(&stkData), "ERROR WHILE TRYING TO OUT RESULT");
    //         break;
    //     case 2:
    //         sscanf(commandLine.str + ind, "%lg%n", &val, &step);
    //         ind += step;
    //         RUN_COMMAND(StackPush(&stkData, val), "ERROR WHILE TRYING TO PUSH");
    //         break;
    //     case 3:
    //         RUN_COMMAND(AddElls(&stkData), "ERROR WHILE TRYING TO ADD ELEMENTS");
    //         break;
    //     case 4:
    //         RUN_COMMAND(SubtrElls(&stkData), "ERROR WHILE TRYING TO SUBTRACT");
    //         break;
    //     case 5:
    //         RUN_COMMAND(MultElls(&stkData), "ERROR WHILE TRYING TO MULTIPLY");
    //         break;
    //     case 6:
    //         RUN_COMMAND(DivElls(&stkData), "ERROR WHILE TRYING TO DIVIDE");
    //         break;
    //     case 7:
    //         RUN_COMMAND(SqrtEll(&stkData), "ERROR WHILE TRYING TO TAKE SQUARE ROOT");
    //         break;
    //     case 8:
    //         RUN_COMMAND(SinEll(&stkData), "ERROR WHILE TRYING TO TAKE SIN");
    //         break;
    //     case 9:
    //         RUN_COMMAND(CosEll(&stkData), "ERROR WHILE TRYING TO TAKE COS");
    //         break;
    //     case 10:
    //         RUN_COMMAND(TanEll(&stkData), "ERROR WHILE TRYING TO TAKE TAN");
    //         break;
    //     default:
    //         return $ERR("WRONG COMMAND TO RUN", WRIN);
    //     }
    // }
    // return OK;
}

WORK_RES SPU_Init(SPU_t *spu, const char *fileName)
{
    assert(spu);
    assert(fileName);

    WORK_RES statusCode = OK;

    spu -> IP      = 0;
    spu -> codeLen = 0;

    if ((statusCode = StackCtor(&(spu -> dataStk), STACK_INIT_CAPACITY)) != OK) {
        return $ERR("ERROR WHILE TRYING TO CONSTRUCT STACK FOR DATA", statusCode);
    }
    if ((statusCode = StackCtor(&(spu -> callStk), STACK_INIT_CAPACITY)) != OK) {
        return $ERR("ERROR WHILE TRYING TO CONSTRUCT STACK FOR CALLS", statusCode);
    }

    RUN_COMMAND(GetCompiledFileInfo(fileName, spu -> code, &(spu -> codeLen)), "CAN'T GET FILE INFO");

    return statusCode;
}

WORK_RES GetCompiledFileInfo(const char *fileName, double *code, size_t *codeLen)
{
    assert(fileName != NULL);
    assert(codeLen);

    WORK_RES statusCode = OK;

    string_t string = {};
    
    RUN_COMMAND(GetFileFull(fileName, &string), "CAN'T GET CODE FROM FILE");

    code = (double *) calloc(string.len, sizeof(double));
    if (code == NULL) return $ERR("NOT ENOUGH MEMORY FOR CODE", NOMEM);
    *codeLen = 0;

    double command = 0;

    size_t ind = 0;
    size_t step = 0;
    while(sscanf(string.str + ind, "%lg%n", &command, &step) == 2) {
        $dbl(command);
        code[(*codeLen)++] = command;
        ind += step;
    }

    code = (double *) realloc(code, sizeof(double) * *codeLen);

    free(string.str);

    return statusCode;
}

WORK_RES StopWork(string_t *commandLine, stack_t *stk)
{
    assert(commandLine);
    assert(stk);

    WORK_RES statusCode = OK;

    free(commandLine -> str);

    RUN_COMMAND(StackDtor(stk), "ERROR WHILE TRUING TO DECONSTRUCT STACK");

    return statusCode;
}

WORK_RES ShowRes(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el = 0;
    RUN_COMMAND(StackPop(stk, &el), "ERROR WHILE TRYING TO POP");

    printf(CYN "Result of calculation is:" COLOR_RESET " <" GRN "%lg" COLOR_RESET ">\n", el);

    return statusCode;
}

WORK_RES AddElls(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el1 = 0, el2 = 0;
    RUN_COMMAND(StackPop(stk, &el1), "ERROR WHILE TRYING TO POP");
    RUN_COMMAND(StackPop(stk, &el2), "ERROR WHILE TRYING TO POP");

    RUN_COMMAND(StackPush(stk, el1 + el2), "ERROR WHILE TRYING TO PUSH");

    return statusCode;
}

WORK_RES SubtrElls(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el1 = 0, el2 = 0;
    RUN_COMMAND(StackPop(stk, &el2), "ERROR WHILE TRYING TO POP");
    RUN_COMMAND(StackPop(stk, &el1), "ERROR WHILE TRYING TO POP");

    RUN_COMMAND(StackPush(stk, el1 - el2), "ERROR WHILE TRYING TO PUSH");

    return statusCode;
}

WORK_RES MultElls(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el1 = 0, el2 = 0;
    RUN_COMMAND(StackPop(stk, &el1), "ERROR WHILE TRYING TO POP");
    RUN_COMMAND(StackPop(stk, &el2), "ERROR WHILE TRYING TO POP");

    RUN_COMMAND(StackPush(stk, el1 * el2), "ERROR WHILE TRYING TO PUSH");

    return statusCode;
}

WORK_RES DivElls(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el1 = 0, el2 = 0;
    RUN_COMMAND(StackPop(stk, &el2), "ERROR WHILE TRYING TO POP");
    RUN_COMMAND(StackPop(stk, &el1), "ERROR WHILE TRYING TO POP");

    if (IsZero(el2)) {
        return $ERR("DIVISION BY ZERO", WRIN);
    }

    RUN_COMMAND(StackPush(stk, el1 / el2), "ERROR WHILE TRYING TO PUSH");

    return statusCode;
}

WORK_RES SqrtEll(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el = 0;
    RUN_COMMAND(StackPop(stk, &el), "ERROR WHILE TRYING TO POP");

    if (el < 0) {
        return $ERR("CAN'T TAKE SQUARE ROOT FROM NEGATIVE INTEGER", WRIN);
    }

    RUN_COMMAND(StackPush(stk, sqrt(el)), "ERROR WHILE TRYING TO PUSH");

    return statusCode;
}

WORK_RES SinEll(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el = 0;
    RUN_COMMAND(StackPop(stk, &el), "ERROR WHILE TRYING TO POP");

    RUN_COMMAND(StackPush(stk, sin(el)), "ERROR WHILE TRYING TO PUSH");

    return statusCode;
}

WORK_RES CosEll(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el = 0;
    RUN_COMMAND(StackPop(stk, &el), "ERROR WHILE TRYING TO POP");

    RUN_COMMAND(StackPush(stk, cos(el)), "ERROR WHILE TRYING TO PUSH");

    return statusCode;
}

WORK_RES TanEll(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el = 0;
    RUN_COMMAND(StackPop(stk, &el), "ERROR WHILE TRYING TO POP");

    RUN_COMMAND(StackPush(stk, tan(el)), "ERROR WHILE TRYING TO PUSH");

    return statusCode;
}
