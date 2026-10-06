#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "basic.h"

#ifdef DEBUGGER
#define ON_DBG(...) __VA_ARGS__
#else
#define ON_DBG(...)
#endif

#ifdef CANARY_PROT
#define ON_CANARY(...) __VA_ARGS__
#else
#define ON_CANARY(...)
#endif

#ifdef HASH_PROT
#define ON_HASH(...) __VA_ARGS__
#else
#define ON_HASH(...)
#endif

#define STACK_CTOR(stk, capacity) StackCtor(stk, capacity \
    ON_DBG(         \
        ,TO_STR(stk), \
        __FILE__,     \
        __LINE__,     \
        __FUNCTION__  \
    ) \
)

#define STACK_PUSH(stk, elem) StackPush(stk, elem ON_DBG(, __FILE__, __LINE__))
#define STACK_POP(stk, elem) StackPop(stk, elem ON_DBG(, __FILE__, __LINE__))

#define HASHER(data, dataLength) djb2((const unsigned char *) data, dataLength)

typedef double stackDataType;
#define FILL_FOR_PRINTF "%d"

ON_DBG(
const stackDataType STACK_EL_POISON = nan;
const stackDataType STACK_CANARY_FIRST = 6767.6767;
const stackDataType STACK_CANARY_SECOND = 6969.6969;
const int BUFF_DUMP_SIZE = 100;

const unsigned long long STACK_STRUCT_LEFT_CANARY = 0xC0FFEEE;
const unsigned long long STACK_STRUCT_RIGHT_CANARY = 0xC0FFFFE;

)


struct stack_t {
    ON_CANARY(unsigned long long leftCanary;)

    stackDataType *data;
    size_t size;
    size_t capacity;

    ON_DBG(
    const char *MyName;
    const char *fileCreationName;
    int   creationLineName;
    const char *creationFunctionName;
    )
    ON_CANARY(
    stackDataType *canaryData;
    )
    ON_HASH(
    unsigned long long hashOfData;
    unsigned long long hashOfStruct;
    )

    ON_CANARY(unsigned long long rightCanary;)
};

WORK_RES StackCtor(
        stack_t *stk, size_t capacity 
        ON_DBG(,const char *Myname, const char *fileCreationName, int creationLineName, const  char *creationFunctionName)
    );
WORK_RES StackDtor(stack_t *stk);

WORK_RES StackPush(stack_t *stk, stackDataType Elem ON_DBG(, const char *fileCall, int lineCall));
WORK_RES ResizeUp(stack_t *stk);

WORK_RES StackPop(stack_t *stk, stackDataType *elem ON_DBG(, const char *fileCall, int lineCall));
WORK_RES ResizeDown(stack_t *stk);

WORK_RES GetStackCapacity(size_t *capacity);

WORK_RES StackIsValid(stack_t stk);

WORK_RES StackDump(stack_t *stk, ErrInfo errInfo);

unsigned long long djb2(const unsigned char *data, size_t dataLength);

#endif
