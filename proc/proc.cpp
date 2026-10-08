#include "proc.h"


WORK_RES MainCalculation(const char *FileName)
{
    assert(FileName);


    WORK_RES statusCode = OK;

    string_t commandLine = {};
    RUN_COMMAND(GetCompiledFileInfo(FileName, &commandLine), "CAN'T GET INFO FROM FILE");
    
    stack_t stkData = {};

    if ((statusCode = StackCtor(&stkData, STACK_INIT_CAPACITY)) != OK) {
        return $ERR("ERROR WHILE TRYING TO CONSTRUCT STACK", statusCode);
    }

    size_t ind = 0;

    int command = -1;
    stackDataType val = 0;

    while (ind <= commandLine.len) {
        size_t step = 0;

        if (sscanf(commandLine.str + ind, "%d%n", &command, &step) <= 0) break;
        ind += step;

        switch (command) {
        case 0:
            return StopWork(&commandLine, &stkData);
        case 1:
            RUN_COMMAND(ShowRes(&stkData), "ERROR WHILE TRYING TO OUT RESULT");
            break;
        case 2:
            sscanf(commandLine.str + ind, "%lg%n", &val, &step);
            ind += step;
            RUN_COMMAND(StackPush(&stkData, val), "ERROR WHILE TRYING TO PUSH");
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
            return $ERR("WRONG COMMAND TO RUN", WRIN);
        }
    }
    return OK;
}

WORK_RES GetCompiledFileInfo(const char *fileName, string_t *string)
{
    assert(fileName != NULL);
    assert(string);

    int fileDes = open(fileName, O_RDONLY, 0);
    if (fileDes == -1) return $ERR("NO FILE TO READ FROM IN GetCompiledFileInfo", FILEERR);

    struct stat st = {};
    if (fstat(fileDes, &st)) return $ERR("ERROR IN FILE WHILE TRYING TO fstat", FILEERR);

    string -> str = (char *) calloc(st.st_size + 1, sizeof(char));
    if (string -> str == NULL) return $ERR("NOT ENOUGH MEMORY TO READ FILE AS FULL", NOMEM);

    int fileLen = read(fileDes, (void *) string -> str, st.st_size);
    if (fileLen < 0) return $ERR("ERROR WHILE READING FROM FILE", FILEERR);

    string -> str = (char *) realloc(string -> str, fileLen + 1);
    *(string -> str + fileLen) = '\0';
    
    string -> len = (size_t) fileLen;

    close(fileDes);

    return OK;
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
