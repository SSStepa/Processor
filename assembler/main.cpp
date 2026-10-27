#include "assembler.h"

int main(const int argc, char *const *argv)
{
    SetUpLog();

    // const char *inFileName = "circle.asm";
    // const char *inFileName = "fact.asm";
    const char *inFileName = "../BadAppleOut.asm";
    const char *outFileName = "c.bin";

    FlagsParse(argc, argv, &inFileName, &outFileName);
    
    Assembler(inFileName, outFileName, "b.txt");

    return 0;
}