#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <sys/stat.h>

#include "../basic.h"

const int  MAX_LABEL_LEN = 20; 

struct commandCode
{
    const char *code;
    const char *name;
};

struct registerCode
{
    char code;
    const char *name;
};

struct label_t {
    char name[MAX_LABEL_LEN];
    int placeToGo;
};

struct labelList_t 
{
    label_t *labels;
    size_t   fullElls;
    size_t   len;
};

struct FileData {
    string_t     asmStartData;

    string_t     binOpcode;
    string_t     textOpcode;
    
    labelList_t  labelsList;
    
    string_t    *indexDyn;
    size_t       indLen; 
    char        *commandLine;
};

/**
 * Command to it's code (sorted in alphabet to bin search)
 */
const commandCode COMMANDS_TO_CODE[] = {
    "0",   "HLT",
    "1",   "OUT",
    "2",   "PUSH", // also PUSH for register is 67
    "3",   "POP",
    "4",   "SUB",
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
    "21",  "RET",
    "22",  "IN",
    "23",  "DRAW",
    "24",  "OUT_C",
    "", ""
};

const registerCode REGISTERS[] = {
     1, "AX",
     2, "BX",
     3, "CX",
     4, "DX",
    -1, ""
};


const int COMMAND_MAX_LEN   = 20;
const int REGISTER_MAX_LEN  = 3;
const int LABELS_START_SIZE = 20;
const char *const LABEL_INIT      = "";

WORK_RES Assembler(const char *inFileName, const char *outBinFileName, const char *outFileName);
WORK_RES GetCompiledCodeLine(FileData *data, bool firstRun);
WORK_RES PutCommandToCode(FileData *data, char *command, int *elementsAmount);

WORK_RES PushCommandToCode(FileData *data, int *elementsAmount);
WORK_RES PopCommandToCode(FileData *data, int *elementsAmount);
WORK_RES PushPopArgParse(char *commandNumber, FileData *data, int *elementsAmount);
WORK_RES GoToArgToCode(FileData *data);

WORK_RES PutLabelIntoList(labelList_t *labelList, char *command, int elementInd);
WORK_RES LabelsInit(labelList_t *labelList);
WORK_RES InitLabelsValues(label_t *labels, size_t len);
WORK_RES GetRegisterByName(char *command, char *regNumber);

WORK_RES ParseFileToLines(FileData *data);
WORK_RES CountEls(size_t *amount, char *arr, size_t  arrLen, char El);
WORK_RES OutFileFull(const char *outBinName, const char *outFileTextName, FileData *data);
WORK_RES ClearData(FileData *data);

#endif