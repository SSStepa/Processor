#include "comp.h"

WORK_RES GetCommand(int *command, double *arg)
{
    assert(command);
    assert(arg);

    char commandLine[COMMAND_MAX_LEN] = {};
    *arg = 0;

    if (scanf("%s %lg", commandLine, arg) > 0) {
        int ind = 0;
        while (COMMANDS[ind].code != -1) {
            if (strcmp(commandLine, COMMANDS[ind].name) == 0) {
                *command = ind;
                break;
            }
            ind++;
        }

    } else {
        return $ERR("WRONG INPUT FROM USER", WRIN);
    }

    return OK;
    
}