#include "assembler.h"

int main()
{
    SetUpLog();

    const char *inFileName = "sqr.asm";
    const char *outFileName = "c.bin";
    
    Assembler(inFileName, outFileName, "b.txt");

    return 0;
}