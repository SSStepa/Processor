#include "../includes/basic.h"

void InfoPrintfInt(int var, const char *varName, const char *file, int line)
{
    fprintf(stderr, YEL "%s, %d:%s %s = <%s%d%s>\n", file, line, COLOR_RESET, varName, CYN, var, COLOR_RESET);
}

void InfoPrintfStr(const char *var, const char *varName, const char *file, int line)
{
    fprintf(stderr, YEL "%s, %d:%s %s = <%s%s%s>\n", file, line, COLOR_RESET, varName, CYN, var, COLOR_RESET);
}

void InfoPrintfC(char var, const char *varName, const char *file, int line)
{
    fprintf(stderr, YEL "%s, %d:%s %s = <%s%c%s>\n", file, line, COLOR_RESET, varName, CYN, var, COLOR_RESET);
}

void InfoPrintfLlu(size_t var, const char *varName, const char *file, int line)
{
    fprintf(stderr, YEL "%s, %d:%s %s = <%s%llu%s>\n", file, line, COLOR_RESET, varName, CYN, var, COLOR_RESET);
}


void InfoPrintfDouble(double var, const char *varName, const char *file, int line)
{
    fprintf(stderr, YEL "%s, %d:%s %s = <%s%lg%s>\n", file, line, COLOR_RESET, varName, CYN, var, COLOR_RESET);
}

WORK_RES ErrorPrintf(const char *errMess, int line, const char *file, WORK_RES ErrCode)
{
    LOG_START();

    #ifdef LOUD
    fprintf(stderr, CYN "%s, %d:%s %s:%d %s%s\n" COLOR_RESET, file, line, COLOR_RESET, TO_STR(ErrCode), ErrCode, RED, errMess);
    #endif

    LOG_PRINTF("%s:%d: %s:%d %s\n", file, line, TO_STR(ErrCode), ErrCode, errMess);

    LOG_END();

    return ErrCode;
}

WORK_RES StartError()
{
    FILE *fileLog = fopen(LOG_FILE, "a");
    if (fileLog == NULL) return $ERR("PROBLEM WITH LOG FILE", FILEERR);

    fwrite("# ", 1, 2, fileLog);

    fclose(fileLog);

    return OK;
}

WORK_RES EndError()
{
    FILE *fileLog = fopen(LOG_FILE, "a");
    if (fileLog == NULL) return $ERR("PROBLEM WITH LOG FILE", FILEERR);

    fwrite("-------------------------------------------------------------------------------\n\n", 1, 80, fileLog);

    fclose(fileLog);

    return OK;
}

WORK_RES SetUpLog()
{
    FILE *fileLog = fopen(LOG_FILE, "a");
    if (fileLog == NULL) return $ERR("PROBLEM WITH LOG FILE", FILEERR);

    fwrite("-------------------------------------------------------------------------------\n", 1, 80, fileLog);
    fwrite("                                 <<NEW RUN>>                                   \n", 1, 80, fileLog);
    fwrite("-------------------------------------------------------------------------------\n\n", 1, 80, fileLog);
    
    fclose(fileLog);
    return OK;
}

WORK_RES EndLog()
{
    FILE *fileLog = fopen(LOG_FILE, "a");

    fwrite("PROGRAM ENDED WITH ALL GOOD", 1, 28, fileLog);

    fclose(fileLog);
    return OK;

}

bool IsZero(double x)
{
    if (fabs(x) <= EPSILON)
        return true;
    else
        return false;
}

