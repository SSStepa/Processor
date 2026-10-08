#include "comp.h"

int main()
{
    SetUpLog();

    const char *inFileName = "a.txt";
    const char *outFileName = "b.txt";
    
    Compiler(inFileName, outFileName);

    return 0;
}