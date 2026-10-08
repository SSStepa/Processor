/**
 * @file basic.h
 * @brief structs, consts and funcs i use daily.
 */
#ifndef BASIC_H
#define BASIC_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief Work of function available results.
 */
enum WORK_RES {
    OK       = 0, // all good
    WRIN     = 1, // function got bad args
    NOMEM    = 2, // not enough memory
    FILEERR  = 4, // no file to open.
 
    STACK_STRUCT     = 10, // problem with struct
    STACK_DATA       = 11, // wrong data format
    STACK_CAP        = 12, // zero capacity
    STACK_OVERFLOW   = 13,  // size > capacity
    STACK_UNDERFLOW  = 14, // size < 0;
    STACK_CANARY_RIP = 15 // rip of stack canary;
};

/**
 * @brief struct to store info about string: length and pointer to first element
 * str - pointer to the first element of the string
 * len - length of the string (with no \0)
 */
struct String {
    char *str;
    size_t len;
};

struct ErrInfo {
    const char *fileCreationName;
    int   creationLineName;
    const char *creationFunctionName;
    WORK_RES err;
    const char *ErrorText;
};

/**
 * Const to say that number is zero(all numbers less than EPSILON are regarded as zeros)
 */
const double EPSILON = 0.0001;

/**
 * function to check if number is zero
 */
bool IsZero(double x);
//------------------------------------------------------------- COLORS ------------------------------------------------------------------------
/**
 * Basic colors
 */
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define COLOR_RESET "\e[0m"

//----------------------------------------------------- PRINTING VARS AND ERRORS ------------------------------------------------------------------------


#define TO_STR(str) #str

const char * const LOG_FILE = "main.log";

void InfoPrintfInt      (int var, const char *varName, const char *file, int line);
void InfoPrintfStr      (const char *var, const char *varName, const char *file, int line);
void InfoPrintfC        (char var, const char *varName, const char *file, int line);
void InfoPrintfDouble   (double var, const char *varName, const char *file, int line);
void InfoPrintfLlu      (size_t var, const char *varName, const char *file, int line);
WORK_RES SetUpLog();
WORK_RES EndLog();

WORK_RES ErrorPrintf(const char *errMess, int line, const char *file, WORK_RES ErrCode);
WORK_RES StartError();
WORK_RES EndError();

#define $int(Variable) InfoPrintfInt(Variable, TO_STR(Variable), __FILE__, __LINE__)
#define $str(Variable) InfoPrintfStr(Variable, TO_STR(Variable), __FILE__, __LINE__)
#define $c(Variable) InfoPrintfC(Variable, TO_STR(Variable), __FILE__, __LINE__)
#define $dbl(Variable) InfoPrintfDouble(Variable, TO_STR(Variable), __FILE__, __LINE__)
#define $llu(Variable) InfoPrintfLlu(Variable, TO_STR(Variable), __FILE__, __LINE__)

#define $ERR(ErrorMessage, ErrCode) ErrorPrintf(ErrorMessage, __LINE__, __FILE__, ErrCode)

#define $ERR_START() StartError()

#define $ERR_END() EndError()

#define ERR_INIT(errInfo, statusCode, message) \
    errInfo.fileCreationName = __FILE__; \
    errInfo.creationLineName = __LINE__; \
    errInfo.creationFunctionName = __FUNCTION__; \
    errInfo.err = statusCode; \
    errInfo.ErrorText = message; 

#define LOG_START() \
    FILE *logFile = fopen(LOG_FILE, "a");

#define LOG_END() \
    fclose(logFile);

#define LOG_PRINTF(...) \
    fprintf(logFile, __VA_ARGS__); \
    fflush(logFile);

#ifdef DEBUGGER
#define MY_ASSERT(...) \
    if (!(__VA_ARGS__)) \
        return $ERR("MY ASSERTION FAILED: WRONG INPUT TO FUNCTION", WRIN); 
#else
#define MY_ASSERT(...) 
#endif


#endif
