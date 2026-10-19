#include "proc.h"

WORK_RES Proc(const char *FileName)
{
    assert(FileName);

    WORK_RES statusCode = OK;

    SPU_t spu = {};
    RUN_COMMAND(SPU_Init(&spu, FileName), "ERROR WHILE TRYING TO INIT SPU");

    while (spu.IP < spu.codeLen) {
        spu.command = *((OpCode *) (spu.code + spu.IP));
        spu.IP++;

        switch (spu.command.code) {
        case HLT:
            return StopWork(&spu);
        case OUT:
            RUN_COMMAND(ShowRes(&spu.dataStk), "ERROR WHILE TRYING TO OUT RESULT");
            break;
        case OUT_C:
            RUN_COMMAND(ShowResAsChar(&spu.dataStk), "ERROR WHILE TRYING TO OUT RESULT AS CHAR");
            break;
        case PUSH:
            RUN_COMMAND(PushElls(&spu), "ERROR WHILE TRYING TO PUSH");
            break;
        case POP:
            RUN_COMMAND(PopElls(&spu), "ERROR WHILE TRYING TO POP");
            break;
        case ADD:
            RUN_COMMAND(AddElls(&spu.dataStk), "ERROR WHILE TRYING TO ADD ELEMENTS");
            break;
        case SUBTR:
            RUN_COMMAND(SubtrElls(&spu.dataStk), "ERROR WHILE TRYING TO SUBTRACT");
            break;
        case MULT:
            RUN_COMMAND(MultElls(&spu.dataStk), "ERROR WHILE TRYING TO MULTIPLY");
            break;
        case DIV:
            RUN_COMMAND(DivElls(&spu.dataStk), "ERROR WHILE TRYING TO DIVIDE");
            break;
        case SQRT:
            RUN_COMMAND(SqrtEll(&spu.dataStk), "ERROR WHILE TRYING TO TAKE SQUARE ROOT");
            break;
        case SIN:
            RUN_COMMAND(SinEll(&spu.dataStk), "ERROR WHILE TRYING TO TAKE SIN");
            break;
        case COS:
            RUN_COMMAND(CosEll(&spu.dataStk), "ERROR WHILE TRYING TO TAKE COS");
            break;
        case TAN:
            RUN_COMMAND(TanEll(&spu.dataStk), "ERROR WHILE TRYING TO TAKE TAN");
            break;
        case JMP:
            RUN_COMMAND(JmpCommand(&spu), "ERROR WHILE TRYING TO JMP");
            break;
        case JA:
            RUN_COMMAND(JaCommand(&spu), "ERROR WHILE TRYING TO JA");
            break;
        case JB:
            RUN_COMMAND(JbCommand(&spu), "ERROR WHILE TRYING TO JB");
            break;
        case JE:
            RUN_COMMAND(JeCommand(&spu), "ERROR WHILE TRYING TO JJE");
            break;
        case JNE:
            RUN_COMMAND(JneCommand(&spu), "ERROR WHILE TRYING TO JNE");
            break;
        case JAE:
            RUN_COMMAND(JaeCommand(&spu), "ERROR WHILE TRYING TO JAE");
            break;
        case JBE:
            RUN_COMMAND(JbeCommand(&spu), "ERROR WHILE TRYING TO JBE");
            break;
        case JT:
            RUN_COMMAND(JtCommand(&spu), "ERROR WHILE TRYING TO JT");
            break;
        case CALL:
            RUN_COMMAND(CallFunctionCommand(&spu), "ERROR WHILE TRYING TO FUNCTION CALL");
            break;
        case RET:
            RUN_COMMAND(RetFunctionCommand(&spu), "ERROR WHILE TRYING TO FUNCTION RETURN");
            break;
        case IN:
            RUN_COMMAND(InCommand(&spu), "ERROR WHILE TRYING TO TAKE USER INPUT");
            break;
        case DRAW:
            RUN_COMMAND(DrawRAMCommand(&spu), "ERROR WHILE TRYING TO DRAW RAM");
            break;
        default:
            return $ERR("WRONG COMMAND TO RUN", WRIN);
        }
    }
    return OK;
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

    RUN_COMMAND(GetCompiledFileInfo(fileName, &(spu -> code), &(spu -> codeLen)), "CAN'T GET FILE INFO");

    return statusCode;
}

WORK_RES PushElls(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    if (!(spu -> command).konst && !(spu -> command).reg && !(spu -> command).mem)
        return $ERR("NOT VALID ARGUMENT TO PUSH", WRIN);

    double res = 0;

    if ((spu -> command).konst) {
        res += *((double *) (spu -> code + spu -> IP));
        spu -> IP += sizeof(double);
    }
    if ((spu -> command).reg) {
        res += (spu -> registers)[(int) (spu -> code)[(spu -> IP)++]];
    }
    if ((spu -> command).mem) {
        res = (spu -> RAM)[(int) res];
    }

    RUN_COMMAND(StackPush(&(spu -> dataStk), res), "ERROR WHILE TRYING TO PUSH ELEMENT TO THE STACK");

    return statusCode;
}

WORK_RES PopElls(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    if (!(spu -> command).konst && !(spu -> command).reg && !(spu -> command).mem)
        return $ERR("NOT VALID ARGUMENT TO POP", WRIN);
    if ((spu -> command).konst && !(spu -> command).reg && !(spu -> command).mem)
        return $ERR("CNAT'T POP CONST", WRIN);
    if ((spu -> command).konst && (spu -> command).reg && !(spu -> command).mem)
        return $ERR("CAN'T POP CONST + REG", WRIN);

    double elem = NAN;
    RUN_COMMAND(StackPop(&(spu -> dataStk), &elem), "ERROR WHILE TRYING TO GET ELEMENT FROM THE STACK");

    double res = 0;
    if ((spu -> command).mem) {
        if ((spu -> command).konst) {
            res += *((double *) (spu -> code + spu -> IP));
            spu -> IP += sizeof(double);
        }
        if ((spu -> command).reg) {
            res += (spu -> registers)[(int) (spu -> code)[(spu -> IP)++]];
        }

        (spu -> RAM)[(int) res] = elem;

    } else {
        res = (spu -> code)[(spu -> IP)++];
        (spu -> registers)[(int) res] = elem;
    }

    return statusCode;
}

WORK_RES GetCompiledFileInfo(const char *fileName, char **code, size_t *codeLen)
{
    assert(fileName != NULL);
    assert(codeLen);

    WORK_RES statusCode = OK;

    string_t string = {};
    
    RUN_COMMAND(GetFileFull(fileName, &string), "CAN'T GET CODE FROM FILE");

    *code = string.str; 
    *codeLen = string.len;

    return statusCode;
}

WORK_RES CallFunctionCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    RUN_COMMAND(StackPush(&(spu -> callStk), (double) (spu -> IP + 1)), "ERROR WHILE TRYING TO PUSH CALL INDEX");
    spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));

    return OK;
}

WORK_RES RetFunctionCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    double elem = 0;

    RUN_COMMAND(StackPop(&(spu -> callStk), &elem), "ERROR WHILE TRYING TO POP RET INDEX");
    spu -> IP = (size_t) elem;

    return OK;
}

WORK_RES JmpCommand(SPU_t *spu)
{
    assert(spu);

    spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));

    return OK;
}

WORK_RES JaCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    double el1 = 0;
    double el2 = 0;

    RUN_COMMAND(StackPop(&(spu -> dataStk), &el1), "CAN'T GET ELEMENT FROM STACK");
    RUN_COMMAND(StackPop(&(spu -> dataStk), &el2), "CAN'T GET ELEMENT FROM STACK");

    if (el1 > el2)
        spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));
    else 
        spu -> IP += sizeof(double);
    
    return OK;
}

WORK_RES JbCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    double el1 = 0;
    double el2 = 0;

    RUN_COMMAND(StackPop(&(spu -> dataStk), &el1), "CAN'T GET ELEMENT FROM STACK");
    RUN_COMMAND(StackPop(&(spu -> dataStk), &el2), "CAN'T GET ELEMENT FROM STACK");

    if (el1 < el2)
        spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));
    else 
        spu -> IP += sizeof(double);
  
    return OK;
}

WORK_RES JeCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    double el1 = 0;
    double el2 = 0;

    RUN_COMMAND(StackPop(&(spu -> dataStk), &el1), "CAN'T GET ELEMENT FROM STACK");
    RUN_COMMAND(StackPop(&(spu -> dataStk), &el2), "CAN'T GET ELEMENT FROM STACK");

    if (IsZero(el1 - el2))
        spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));
    else 
        spu -> IP += sizeof(double);
    
    return OK;
}

WORK_RES JaeCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    double el1 = 0;
    double el2 = 0;

    RUN_COMMAND(StackPop(&(spu -> dataStk), &el1), "CAN'T GET ELEMENT FROM STACK");
    RUN_COMMAND(StackPop(&(spu -> dataStk), &el2), "CAN'T GET ELEMENT FROM STACK");

    if (el1 > el2 || IsZero(el1 - el2))
        spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));
    else 
        spu -> IP += sizeof(double);
    
    return OK;
}

WORK_RES JbeCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    double el1 = 0;
    double el2 = 0;

    RUN_COMMAND(StackPop(&(spu -> dataStk), &el1), "CAN'T GET ELEMENT FROM STACK");
    RUN_COMMAND(StackPop(&(spu -> dataStk), &el2), "CAN'T GET ELEMENT FROM STACK");

    if (el1 < el2 || IsZero(el1 - el2))
        spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));
    else 
        spu -> IP += sizeof(double);
    
    return OK;
}

WORK_RES JneCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    double el1 = 0;
    double el2 = 0;

    RUN_COMMAND(StackPop(&(spu -> dataStk), &el1), "CAN'T GET ELEMENT FROM STACK");
    RUN_COMMAND(StackPop(&(spu -> dataStk), &el2), "CAN'T GET ELEMENT FROM STACK");

    if (!IsZero(el1 - el2))
        spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));
    else 
        spu -> IP += sizeof(double);
    
    return OK;
}

WORK_RES JtCommand(SPU_t *spu)
{
    assert(spu);

    time_t t = time(NULL);
    tm *now = localtime(&t);

    if (now -> tm_wday == 3) // jump on tuesdays
        spu -> IP = (size_t) *((double *) (spu -> code + spu -> IP));
    else 
        spu -> IP += sizeof(double);
    
    return OK;
}

WORK_RES StopWork(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;

    free((spu -> code));

    RUN_COMMAND(StackDtor(&(spu -> dataStk)), "ERROR WHILE TRUING TO DECONSTRUCT STACK");
    RUN_COMMAND(StackDtor(&(spu -> callStk)), "ERROR WHILE TRUING TO DECONSTRUCT STACK");

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

WORK_RES ShowResAsChar(stack_t *stk)
{
    assert(stk);

    WORK_RES statusCode = OK;

    stackDataType el = 0;
    RUN_COMMAND(StackPop(stk, &el), "ERROR WHILE TRYING TO POP");

    printf(" %c\n", (char) el);

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

WORK_RES InCommand(SPU_t *spu)
{
    assert(spu);

    WORK_RES statusCode = OK;
    printf("Your number is: ");

    double elem = 0;

    if (scanf("%lg", &elem) != 1)
        return $ERR("ERROR WHILE TRYING TO GET ELEMENT FROM USER", WRIN);
    
    RUN_COMMAND(StackPush(&(spu -> dataStk), elem), "ERROR WHILE TRYING TO PUSH IN ELEMENT");

    return statusCode;
}

WORK_RES DrawRAMCommand(SPU_t *spu)
{
    assert(spu);

    for (size_t ind = 0; ind < RAM_INIT_SIZE; ind++)
        printf("%c", (char) (spu -> RAM)[ind]);

    printf(GRN "That is all \n" COLOR_RESET);

    return OK;
}
