#include "proc.h"

int main(const int argc, char *const *argv)
{
    SetUpLog();
    
    const char *inFileName = "";
    const char *outFileName = "../assembler/c.bin";

    FlagsParse(argc, argv, &inFileName, &outFileName);

    Proc(outFileName);

    return 0;
}