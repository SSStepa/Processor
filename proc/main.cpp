#include "proc.h"

int main()
{
    SetUpLog();
    
    const char *fileName = "b.txt";

    MainCalculation(fileName);

    return 0;
}