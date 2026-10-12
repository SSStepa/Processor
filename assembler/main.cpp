#include "assembler.h"

int main()
{
    SetUpLog();

    const char *inFileName = "a.txt";
    const char *outFileName = "b.txt";
    
    Assembler(inFileName, outFileName);

    return 0;
}