#include "proc.h"

int main()
{
    SetUpLog();
    
    const char *fileName = "b.txt";

    Proc(fileName);

    return 0;
}