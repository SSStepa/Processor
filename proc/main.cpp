#include "proc.h"

// TODO:  push pop arg parse function
// TODO: magic numbers to consts
int main()
{
    SetUpLog();
    
    const char *fileName = "../assembler/c.bin";

    Proc(fileName);

    return 0;
}