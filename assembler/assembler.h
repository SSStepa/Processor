#ifndef PROC_H
#define PROC_H

#include <stdio.h>
#include <sys/stat.h>

#include "../basic.h"

struct commandCode
{
    const char *code;
    const char *name;
};

struct registerCode
{
    const char *code;
    const char *name;
};

struct labelList_t 
{
    int *labels;
    size_t len;
};

// struct FileData {
//     WORK_RES  ErrCode;

//     char      *dataPtr;
//     size_t    dataLen;
    
//     char *binOpcode;
//     char *usu
    
//     string_t    *firstLine;
//     string_t    *indexDyn;
//     size_t    indLen; 
// };

/**
 * Command to it's code (sorted in alphabet to bin search)
 */
const commandCode COMMANDS_TO_CODE[] = {
    "0",   "HLT",
    "1",   "OUT",
    "2",   "PUSH", // also PUSH for register is 67
    "3",   "POP",
    "4",   "SUBTR",
    "5",   "MULT",
    "6",   "DIV",
    "7",   "SQRT",
    "8",   "SIN",
    "9",   "COS",
    "10",  "TAN",
    "11",  "ADD",
    "12",  "JMP",
    "13",  "JA",
    "14",  "JB",
    "15",  "JE",
    "16",  "JNE",
    "17",  "JAE",
    "18",  "JBE",
    "19",  "JT",
    "20",  "CALL",
    "21",  "RET"
    "22",  "IN"
};

const registerCode REGISTERS[] = {
    "1", "AX",
    "2", "BX",
    "3", "CX",
    "4", "DX",
    "", ""
};


const int COMMAND_MAX_LEN   = 20;
const int LABELS_START_SIZE = 5;
const int LABEL_INIT        = -1;

WORK_RES Assembler(const char *inFileName, const char *outFileName);
WORK_RES GetCompiledCodeLine(string_t *inLine, string_t *outLine, bool firstRun, labelList_t *labelsList);
WORK_RES PutCommandToCode(char *inLine, char *outLine, char *command, size_t *ind, int *elementsAmount, labelList_t *labelList);

WORK_RES PushCommandToCode(char *inLine, char *outLine, size_t *ind);
WORK_RES PopCommandToCode(char *inLine, char *outLine, size_t *ind);
WORK_RES GoToArgToCode(char *inLine, char *outLine, size_t *ind, labelList_t *labelList);

WORK_RES PutLabelIntoList(labelList_t *labelList, char *command, int elementInd);
WORK_RES LabelsInit(labelList_t *labelList);
WORK_RES InitLabelsValues(int *labels, size_t len);

#endif