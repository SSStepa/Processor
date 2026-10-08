#include "proc.h"


WORK_RES MainCalculation(const char *FileName)
{
    assert(FileName);


    WORK_RES statusCode = OK;
    double *commandLine = GetCompiledFileInfo(FileName, &statusCode);
    if (statusCode != OK) {
        return $ERR("ERROR WHILE GETTING INFO FROM COMPILED FILE", statusCode);
    }
    
    stack_t stkData = {};

    if ((statusCode = StackCtor(&stkData, STACK_INIT_CAPACITY)) != OK) {
        return $ERR("ERROR WHILE TRYING TO CONSTRUCT STACK", statusCode);
    } 

    double *command = commandLine;

    while (*command != 0) {
        switch ((int) *command) {
        case 0:
            return StopWork(commandLine, &stkData);
        case 1:
            RUN_COMMAND(ShowRes(&stkData), "ERROR WHILE TRYING TO OUT RESULT");
            break;
        case 2:
            RUN_COMMAND(StackPush(&stkData, *(++command)), "ERROR WHILE TRYING TO PUSH");
            break;
        case 3:
            RUN_COMMAND(AddElls(&stkData), "ERROR WHILE TRYING TO ADD ELEMENTS");
            break;
        case 4:
            RUN_COMMAND(SubtrElls(&stkData), "ERROR WHILE TRYING TO SUBTRACT");
            break;
        case 5:
            RUN_COMMAND(MultElls(&stkData), "ERROR WHILE TRYING TO MULTIPLY");
            break;
        case 6:
            RUN_COMMAND(DivElls(&stkData), "ERROR WHILE TRYING TO DIVIDE");
            break;
        case 7:
            RUN_COMMAND(SqrtEll(&stkData), "ERROR WHILE TRYING TO TAKE SQUARE ROOT");
            break;
        case 8:
            RUN_COMMAND(SinEll(&stkData), "ERROR WHILE TRYING TO TAKE SIN");
            break;
        case 9:
            RUN_COMMAND(CosEll(&stkData), "ERROR WHILE TRYING TO TAKE COS");
            break;
        case 10:
            RUN_COMMAND(TanEll(&stkData), "ERROR WHILE TRYING TO TAKE TAN");
            break;
        default:
            break;
        }

        

        command++;
    }

}

WORK_RES StopWork(double *commandLine, stack_t *stk)
{
    assert(commandLine);
    assert(stk);

    WORK_RES statusCode = OK;

    free(commandLine);

    RUN_COMMAND(StackDtor(stk), "ERROR WHILE TRUING TO DECONSTRUCT STACK");

    return statusCode;
}

WORK_RES ShowRes(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el = 0;
    RUN_COMMAND(StackPop(stk, &el), "ERROR WHILE TRYING TO POP");

    printf(CYN "Result of calculation is: <" GRN "%lg" COLOR_RESET ">\n", el);

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
