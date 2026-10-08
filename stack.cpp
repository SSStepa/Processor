#include "stack.h"

WORK_RES StackCtor(
        stack_t *stk, size_t capacity 
        ON_DBG(, const char *MyName, const char *fileCreationName, int creationLineName, const  char *creationFunctionName)
        )
{
    MY_ASSERT(stk != NULL);

    ON_DBG(
        MY_ASSERT(fileCreationName);
        MY_ASSERT(creationFunctionName);
        MY_ASSERT(MyName);
    )

    if (capacity == 0) {
        $ERR_START();
        return $ERR("CAN'T CREATE STACK WITH 0 CAPACITY", WRIN);
    }

    if (stk -> capacity != 0 || stk -> data != 0 || stk -> size != 0) {
        $ERR_START();
        return $ERR("INCORRECT STACK TO INITIALIZE", WRIN);
    }

    stk -> capacity = capacity;
    stk -> data = (stackDataType *) calloc(capacity ON_CANARY(+2), sizeof(stackDataType));
    if (stk -> data == NULL) {
        $ERR_START();
        return $ERR("NOT ENOUGH MEMORY TO CREATE STACK", NOMEM);
    }

    ON_CANARY(
        stk -> canaryData = stk -> data;
        stk -> data++;
        *(stk -> canaryData) = STACK_CANARY_FIRST;
        *(stk -> data + stk -> capacity) = STACK_CANARY_SECOND;

        stk -> leftCanary = STACK_STRUCT_LEFT_CANARY;
        stk -> rightCanary = STACK_STRUCT_RIGHT_CANARY;
    )
    ON_DBG(
        stk -> MyName = MyName;
        stk -> fileCreationName = fileCreationName;
        stk -> creationLineName = creationLineName;
        stk -> creationFunctionName = creationFunctionName;

        for (size_t ind = 0; ind < capacity; ind++) {
            (stk -> data)[ind] = STACK_EL_POISON;
        }
    )
    ON_HASH(

        stk -> hashOfData   = HASHER(stk -> data, (stk -> capacity) * sizeof(stackDataType));
        stk -> hashOfStruct = HASHER(stk, sizeof(stack_t));
    )

    WORK_RES statusCode = OK;
    if ((statusCode = StackIsValid(*stk)) != OK) {
        ON_DBG(
            ErrInfo errInfo = {};
            ERR_INIT(errInfo, statusCode, "INCORRECT STACK AFTER CREATION");
            StackDump(stk, errInfo);
        )
        
        return $ERR("WRONG DATA IN STACK", statusCode);
    }

    return OK;
}

WORK_RES StackPush(stack_t *stk, stackDataType Elem ON_DBG(, const char *fileCall, int lineCall)) 
{
    MY_ASSERT(stk);

    WORK_RES statusCode = OK;

    if ((statusCode = StackIsValid(*stk)) != OK) {
        ON_DBG(
            LOG_START();
            LOG_PRINTF("Error in push called from %s:%d", fileCall, lineCall);
            LOG_END();
        )
        return $ERR("NOT VALID STACK GOT BY FUNCTION", statusCode);
    }

    if (stk -> capacity == stk -> size) {
        if ((statusCode = ResizeUp(stk)) != OK)
            return statusCode;
    }

    (stk -> data)[(stk -> size)++] = Elem;

    ON_HASH(
        stk -> hashOfData = HASHER(stk -> data, (stk -> capacity) * sizeof(stackDataType));
        stk -> hashOfStruct = 0;
        stk -> hashOfStruct = HASHER(stk, sizeof(stack_t));
    )

    if ((statusCode = StackIsValid(*stk)) != OK) {
        ON_DBG(
            ErrInfo errInfo = {};
            ERR_INIT(errInfo, statusCode, "UNVALID STACK WHILE PUSHING");
            StackDump(stk, errInfo);
            LOG_START();
            LOG_PRINTF("Error in push called from %s:%d", fileCall, lineCall);
            LOG_END();
        )
        return $ERR("NOT VALID STACK AFTER PUSH", statusCode);
    }

    return OK;
}

WORK_RES ResizeUp(stack_t *stk)
{
    MY_ASSERT(stk);
    
    stk -> capacity *= 2;
    
    stackDataType *temp = (stackDataType *) realloc(ON_DBG(--)(stk -> data), (stk -> capacity ON_DBG(+2))*sizeof(stackDataType));
    if (temp == NULL) {
        ON_DBG(
            ErrInfo errInfo = {};
            ERR_INIT(errInfo, NOMEM, "NOT ENOUGH MEMORY");
            StackDump(stk, errInfo);
        )
        return $ERR("NOT ENOUGH MEMORY", NOMEM);
    }
    stk -> data = temp;

    ON_CANARY(
        stk -> canaryData = (stk -> data)++;
        *(stk -> canaryData) = STACK_CANARY_FIRST;
        *(stk -> data + stk -> capacity) = STACK_CANARY_SECOND;

        for (size_t ind = stk -> size; ind < stk -> capacity; ind++) {
            (stk -> data)[ind] = STACK_EL_POISON;
        }
    )
    return OK;

}

WORK_RES StackPop(stack_t *stk, stackDataType *elem ON_DBG(, const char *fileCall, int lineCall))
{
    MY_ASSERT(stk);
    MY_ASSERT(elem); 

    WORK_RES statusCode = OK;

    if ((statusCode = StackIsValid(*stk)) != OK) {
        ON_DBG(
            LOG_START();
            LOG_PRINTF("Error in pop called from %s:%d", fileCall, lineCall);
            LOG_END();

            ErrInfo errInfo = {};
            ERR_INIT(errInfo, statusCode, "INCORRECT STACK WHILE TRYING TO POP");
            StackDump(stk, errInfo);
        )
        return $ERR("NOT VALID STACK GOT BY FUNCTION", statusCode);
    }

    if (stk -> size == 0) {
        ON_DBG(
            ErrInfo errInfo = {};
            ERR_INIT(errInfo, STACK_UNDERFLOW, "POP FROM STACK WITH 0 SIZE");
            StackDump(stk, errInfo);
        )
        return $ERR("CAN'T GET ELEMENT FROM STACK WITHOUT ELEMENTS", STACK_UNDERFLOW);
    }

    if ((stk ->capacity)/4 > stk -> size) {
        if ((statusCode = ResizeDown(stk)) != OK) {
            return statusCode;
        }
    }

    *elem = (stk -> data)[--(stk -> size)];

    ON_HASH(
        (stk -> data)[(stk -> size)] = STACK_EL_POISON;
        stk -> hashOfData = HASHER(stk -> data, (stk -> capacity) * sizeof(stackDataType));
        stk -> hashOfStruct = 0;
        stk -> hashOfStruct = HASHER(stk, sizeof(stack_t));
    )

    if ((statusCode = StackIsValid(*stk)) != OK) {
        ON_DBG(
            LOG_START();
            LOG_PRINTF("Error in pop called from %s:%d", fileCall, lineCall);
            LOG_END();

            ErrInfo errInfo = {};
            ERR_INIT(errInfo, statusCode, "INCORRECT STACK WHILE TRYING TO POP");
            StackDump(stk, errInfo);
        )
        return $ERR("NOT VALID STACK AFTER POP", statusCode);
    }

    return OK;
}

WORK_RES ResizeDown(stack_t *stk)
{
    MY_ASSERT(stk);

    stk -> capacity = stk -> size;
    
    stackDataType *temp = (stackDataType *) realloc(ON_DBG(--)(stk -> data), (stk -> capacity ON_DBG(+2))*sizeof(stackDataType));
    if (temp == NULL) {
        ON_DBG(
            ErrInfo errInfo = {};
            ERR_INIT(errInfo, NOMEM, "PROBLEM WHILE TRYING TO REALLOC TO SMALLER SIZE");
            StackDump(stk, errInfo);
        )
        return $ERR("PROBLEM WHILE TRYING TO REALLOC TO SMALLER SIZE", NOMEM);
    }
    stk -> data = temp;

    ON_CANARY(
        stk -> canaryData = (stk -> data)++;
        *(stk -> canaryData) = STACK_CANARY_FIRST;
        *(stk -> data + stk -> capacity) = STACK_CANARY_SECOND;
    )

    return OK;
}

WORK_RES StackDtor(stack_t *stk)
{
    MY_ASSERT(stk);

    WORK_RES statusCode = OK;
    if ((statusCode = StackIsValid(*stk)) != OK) {
        ON_DBG(
           ErrInfo errInfo = {};
            ERR_INIT(errInfo, WRIN, "INCORRECT STACK TO DELETE");
            StackDump(stk, errInfo);
        )
        return $ERR("WRONG DATA IN STACK", statusCode);
    }

    free(ON_CANARY(--)(stk -> data));
    stk -> data = 0;
    stk -> capacity = 0;
    stk -> size = 0;
    ON_DBG(
        stk -> canaryData = 0;
        stk -> hashOfData = 0;
        stk -> hashOfStruct = 0;
    )

    return OK;
}

WORK_RES GetStackCapacity(size_t *capacity)
{
    MY_ASSERT(capacity);

    printf("Hi. What capacity of the stack do you need: ");

    if (scanf("%llu", capacity) == 1)
        return OK;
    else
        return WRIN;

}

WORK_RES StackIsValid(stack_t stk)
{
    if (stk.data == 0) {
        $ERR_START();
        return $ERR("STACK DATA PTR IS 0", STACK_DATA);
    }

    if (stk.capacity == 0) {
        $ERR_START();
        return $ERR("STACK CAPACITY IS 0", STACK_CAP);
    }

    if (stk.size > stk.capacity) {
        $ERR_START();
        return $ERR("STACK SIZE OF BIGGER THAN CAPACITY", STACK_OVERFLOW);
    }

    ON_DBG(
    if (*stk.canaryData != STACK_CANARY_FIRST) {
        $ERR_START();
        return $ERR("LEFT CANARY IN DATA IS KILLED", STACK_CANARY_RIP);
    }
    if (*(stk.data + stk.capacity) != STACK_CANARY_SECOND) {
        $ERR_START();
        return $ERR("RIGHT CANARY IN DATA IS KILLED", STACK_CANARY_RIP);
    }
    if (stk.leftCanary != STACK_STRUCT_LEFT_CANARY) {
        $ERR_START();
        return $ERR("LEFT STRUCT CANARY IS KILLED", STACK_CANARY_RIP);
    }
    if (stk.rightCanary != STACK_STRUCT_RIGHT_CANARY) {
        $ERR_START();
        return $ERR("RIGHT STRUCT CANARY IS KILLED", STACK_CANARY_RIP);
    }
    if (stk.hashOfData != HASHER(stk.data, stk.capacity * sizeof(stackDataType))) {
        $ERR_START();
        return $ERR("WRONG HASH FOR DATA", STACK_DATA);
    }

    unsigned long long hashOfStruct = stk.hashOfStruct;
    stk.hashOfStruct = 0;
    if (HASHER((&stk), sizeof(stack_t)) != hashOfStruct) {
        $ERR_START();
        return $ERR("WRONG HASH FOR STRUCT", STACK_STRUCT);
    }
    )
    return OK;
}


WORK_RES StackDump(ON_DBG(stack_t *stk, ErrInfo errInfo))
{
    ON_DBG(
    MY_ASSERT(stk != NULL);

    LOG_START();

    LOG_PRINTF("\nBEGIN OF DUMP\n\n");
    LOG_PRINTF("%s %s\n", __DATE__, __TIME__);

    LOG_PRINTF("Dump was called in %s %s:%d with error code %d because of %s\n", 
        errInfo.fileCreationName,
        errInfo.creationFunctionName,
        errInfo.creationLineName,
        errInfo.err,
        errInfo.ErrorText
    );

    if (errInfo.err == STACK_DATA) {
        LOG_PRINTF("DUMP CAN'T BE CALLED: ERROR IS STACK_DATA");
        LOG_PRINTF("%s %s\n", __DATE__, __TIME__);
        LOG_PRINTF("\nEND OF DUMP\n\n");

        return OK;
    }

    LOG_PRINTF("\nHi there! I'm %s[%p] and I was created in %s %s:%d\n", 
        &(stk -> MyName)[1], // to skip &
        stk, 
        stk -> fileCreationName, 
        stk -> creationFunctionName, 
        stk -> creationLineName
    );

    LOG_PRINTF("capacity = <%llu> \nsize = <%llu>\n\n", stk -> capacity, stk -> size);
    LOG_PRINTF("data is on [%p]\n", stk -> data);

    for (size_t ind = 0; ind < stk -> capacity; ind++) {
        if (IsZero((stk -> data)[ind] - STACK_EL_POISON) || isnan((stk -> data)[ind])) {
            LOG_PRINTF("* [%llu] ", ind);
            LOG_PRINTF("<" FILL_FOR_PRINTF ">", (stk -> data)[ind]);
            if (strcmp(FILL_FOR_PRINTF, "%c") == 0)
                LOG_PRINTF(" (%d) ", (stk -> data)[ind]);
            LOG_PRINTF("POISON\n");

        } else {
            LOG_PRINTF("  [%llu]", ind);
            LOG_PRINTF("<" FILL_FOR_PRINTF ">", (stk -> data)[ind]);
            if (strcmp(FILL_FOR_PRINTF, "%c") == 0)
                LOG_PRINTF(" (%d)", (stk -> data)[ind]);
            LOG_PRINTF("\n");
        }
    }

    LOG_PRINTF("Expected and got canary: \n");
    LOG_PRINTF("left in data: <" FILL_FOR_PRINTF "> and <" FILL_FOR_PRINTF ">\n", STACK_CANARY_FIRST, *(stk -> canaryData));
    LOG_PRINTF("right in data: <" FILL_FOR_PRINTF "> and <" FILL_FOR_PRINTF ">\n", STACK_CANARY_FIRST, *(stk -> data + stk -> capacity));
    LOG_PRINTF("left in struct: <0x%llx> and <0x%llx>\n", STACK_STRUCT_LEFT_CANARY, stk -> leftCanary);
    LOG_PRINTF("right in struct: <0x%llx> and <0x%llx>\n", STACK_STRUCT_RIGHT_CANARY, stk -> rightCanary);
    
    LOG_PRINTF("\nHash of :\n")
    LOG_PRINTF("- DATA:   <0x%llx>\n", stk -> hashOfData);
    LOG_PRINTF("- STRUCT: <0x%llx>\n", stk -> hashOfStruct);

    LOG_PRINTF("\n%s %s\n", __DATE__, __TIME__);
    LOG_PRINTF("END OF DUMP\n\n");
    
    LOG_END();

    #ifdef LOUD

    printf("Hi there! I'm %s[%s%p%s] and I was created in %s %s:%d\n", 
        (stk -> MyName),
        YEL,
        stk, 
        COLOR_RESET,
        stk -> fileCreationName, 
        stk -> creationFunctionName, 
        stk -> creationLineName
    );

    for (size_t ind = 0; ind < stk -> capacity; ind++) {
        if ((stk -> data)[ind] == STACK_EL_POISON) {
            printf("* [%s%llu%s] <%s" , CYN, ind, COLOR_RESET, GRN);
            printf(FILL_FOR_PRINTF, (stk -> data)[ind]);
            printf("%s> POISON\n", COLOR_RESET);
        } else {
            printf("  [%s%llu%s] <%s" , CYN, ind, COLOR_RESET, GRN);
            printf(FILL_FOR_PRINTF , (stk -> data)[ind]);
            printf("%s>\n", COLOR_RESET);
        }
    }
    printf("capacity is <%s%llu%s> and size if <%s%llu%s>\n\n", GRN, stk -> capacity, COLOR_RESET, GRN, stk -> size, COLOR_RESET);

    #endif
    )
    
    return OK;
}

unsigned long long djb2(const unsigned char *data, size_t dataLength) 
{
    unsigned long long hash = 5381;

    for (size_t ind = 0; ind < dataLength; ind++) {
        hash = ((hash << 5) + hash) + data[ind];
    }

    return hash;
}
