#include "assembler.h"

int main()
{
    SetUpLog();

    // const char *inFileName = "circle.asm";
    // const char *inFileName = "fact.asm";
    const char *inFileName = "../BadAppleOut.asm";
    const char *outFileName = "c.bin";
    
    Assembler(inFileName, outFileName, "b.txt");

    return 0;
}