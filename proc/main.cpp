#include "proc.h"

int main()
{
    SetUpLog();
    
    const char *fileName = "../assembler/c.bin";

    Proc(fileName);

    return 0;
}